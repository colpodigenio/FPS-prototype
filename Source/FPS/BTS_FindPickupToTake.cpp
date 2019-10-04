// Fill out your copyright notice in the Description page of Project Settings.

#include "BTS_FindPickupToTake.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"
#include "WorldCollision.h"
#include "Kismet/GameplayStatics.h"
#include "Pickup.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Weapon.h"
#include "Kismet/KismetMathLibrary.h"

UBTS_FindPickupToTake::UBTS_FindPickupToTake()
{
	bNotifyBecomeRelevant = true;
	bCallTickOnSearchStart = true;
	bIsPickupMemoryTimerRun = false;
	MemoryDuration = 20;
	RangeOfVision = 10000.0f;
	VisionHalfAngle = 50;
}

void UBTS_FindPickupToTake::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	OwnerPawn = Cast<AFPSCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	Blackboard = OwnerComp.GetBlackboardComponent();
}

void UBTS_FindPickupToTake::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (!OwnerPawn.Get() || !Blackboard.Get()) return;

	//TArray<FHitResult> VisibleActors = GetAllVisibleActorsExceptCharacters();
	
	AddVisiblePickupsToKnown(GetAllVisiblePickups());
	SetTargetPickup();
	
	if (TargetPickup)
		Blackboard->SetValueAsObject(TargetPickupKey.SelectedKeyName, TargetPickup);

	if (!bIsPickupMemoryTimerRun)
	{
		CyclePickupMemoryTimer();
		bIsPickupMemoryTimerRun = true;
	}
}


TArray<APickup*> UBTS_FindPickupToTake::GetAllVisiblePickups()
{
	TArray<AActor*> AllPickups;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickup::StaticClass(), AllPickups);
	TArray<APickup*> VisiblePickups;
	FVector BotForwardVector = OwnerPawn->GetActorForwardVector();
	for (AActor* Pickup : AllPickups)
	{
		FVector BotVectorToPickup = (Pickup->GetActorLocation() - OwnerPawn->GetActorLocation()).GetSafeNormal();
		float PickupAngle = UKismetMathLibrary::DegAcos(FVector::DotProduct(BotForwardVector, BotVectorToPickup)); // angle between pickup's position and bot's forward vector
		float DistanceToPickup = (Pickup->GetActorLocation() - OwnerPawn->GetActorLocation()).Size();
		if (PickupAngle <= VisionHalfAngle && DistanceToPickup <= RangeOfVision && Cast<APickup>(Pickup)->IsPickupActive())
			VisiblePickups.Emplace(Cast<APickup>(Pickup));
	}
	return VisiblePickups;
}

void UBTS_FindPickupToTake::AddVisiblePickupsToKnown(TArray<APickup*> VisiblePickups)
{
	for (APickup* Pickup : VisiblePickups)
	{
		FHitResult HitRes;
		FVector StartPoint = OwnerPawn->GetActorLocation();
		FVector EndPoint = Pickup->GetActorLocation();
		float DistanceToPickup = (StartPoint - EndPoint).Size();
		if (!GetWorld()->LineTraceSingleByChannel(HitRes, StartPoint, EndPoint, ECC_Visibility))
		{
			FPickupData PickupData = FPickupData(Pickup, DistanceToPickup, MemoryDuration);
			KnownPickups.Add(Pickup->PickupID, PickupData);
		}
	}
}

void UBTS_FindPickupToTake::SetTargetPickup()
{
	float LastPickupValue = 0;
	for (auto& Element : KnownPickups)
	{
		FPickupData PickupData = Element.Value;
		EPickupType PickupType = PickupData.PickupRef->GetPickupType();
		
		switch (PickupType)
		{
		case EPickupType::Health:
			CalculateNeedValueAndSetTargetPickup(PickupData, LastPickupValue, Blackboard->GetValueAsFloat(HealthNeedRatioKey.SelectedKeyName));
			break;
		case EPickupType::Armor:
			CalculateNeedValueAndSetTargetPickup(PickupData, LastPickupValue, Blackboard->GetValueAsFloat(ArmorNeedRatioKey.SelectedKeyName));
			break;
		case EPickupType::AssaultRifleAmmo:
			CalculateNeedValueAndSetTargetPickup(PickupData, LastPickupValue, Blackboard->GetValueAsFloat(AssaultRifleAmmoNeedRatioKey.SelectedKeyName));
			break;
		case EPickupType::ShotgunAmmo:
			CalculateNeedValueAndSetTargetPickup(PickupData, LastPickupValue, Blackboard->GetValueAsFloat(ShotgunAmmoNeedRatioKey.SelectedKeyName));
			break;
		case EPickupType::RocketLauncherAmmo:
			CalculateNeedValueAndSetTargetPickup(PickupData, LastPickupValue, Blackboard->GetValueAsFloat(RocketLauncherAmmoNeedRatioKey.SelectedKeyName));
			break;
		case EPickupType::Weapon:
			CalculateWeaponNeedValue(PickupData, LastPickupValue);
			break;
		case EPickupType::Powerup:
			CalculateNeedValueAndSetTargetPickup(PickupData, LastPickupValue, Blackboard->GetValueAsFloat(PowerupNeedRatioKey.SelectedKeyName));
			break;
		}
	}
}

void UBTS_FindPickupToTake::CalculateNeedValueAndSetTargetPickup(FPickupData PickupData, float &LastPickupValue, float NeedRatio)
{
	float PickupValue;
	PickupValue = (RangeOfVision / PickupData.DistanceToPickup) * static_cast<float>(PickupData.PickupRef->IsPickupActive()) * NeedRatio * PickupData.PickupRef->GetPickupValue();
	UE_LOG(LogTemp, Warning, TEXT("%s --- %f ^^^ LastPickup --- %f"), *PickupData.PickupRef->GetName(), PickupValue, LastPickupValue)
	if (PickupValue > LastPickupValue)
	{
		LastPickupValue = PickupValue;
		TargetPickup = PickupData.PickupRef;
	}
}

void UBTS_FindPickupToTake::CalculateWeaponNeedValue(FPickupData PickupData, float &LastPickupValue)
{
	switch (Cast<AWeapon>(PickupData.PickupRef)->GetWeaponType())
	{
	case(EWeaponType::AssaultRifle):
		CalculateNeedValueAndSetTargetPickup(PickupData, LastPickupValue, Blackboard->GetValueAsFloat(AssaultRifleNeedRatioKey.SelectedKeyName));
		break;
	case(EWeaponType::Shotgun):
		CalculateNeedValueAndSetTargetPickup(PickupData, LastPickupValue, Blackboard->GetValueAsFloat(ShotgunNeedRatioKey.SelectedKeyName));
		break;
	case(EWeaponType::RocketLauncher):
		CalculateNeedValueAndSetTargetPickup(PickupData, LastPickupValue, Blackboard->GetValueAsFloat(RocketLauncherNeedRatioKey.SelectedKeyName));
		break;
	}
}

void UBTS_FindPickupToTake::CyclePickupMemoryTimer()
{
	TArray<FPickupData> KnownPickupsArr;
	KnownPickups.GenerateValueArray(KnownPickupsArr);
	for (auto& Element : KnownPickupsArr)
	{
		Element.MemoryDuration--;
		KnownPickups.Add(Element.PickupRef->PickupID, Element);
		if (Element.MemoryDuration <= 0 || !Element.PickupRef->IsPickupActive())
			KnownPickups.Remove(Element.PickupRef->PickupID);
	}
	FTimerHandle MemoryTimer;
	GetWorld()->GetTimerManager().SetTimer(MemoryTimer, this, &UBTS_FindPickupToTake::CyclePickupMemoryTimer, 1.0f);
}