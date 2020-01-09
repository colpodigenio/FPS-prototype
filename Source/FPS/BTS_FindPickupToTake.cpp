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
#include "PickupRespawnPoint.h"

UBTS_FindPickupToTake::UBTS_FindPickupToTake()
{
	bNotifyBecomeRelevant = true;
	bCallTickOnSearchStart = true;
	bIsPickupMemoryTimerRun = false;
	MemoryDuration = 20;
	RangeOfVision = 10000.0f;
	VisionHalfAngle = 50;
	ChosenPickupNeedValue = 0.0f;
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

	ChosenPickupNeedValue = 0.0f;

	if (!OwnerPawn.Get() || !Blackboard.Get()) return; // needed because first tick starts before OnBecomeRelevant
		
	CalculateNeeds();
	FilterVisiblePickups(GetAllPickupsInFieldOfView());
	SetTargetPickup();
	//UE_LOG(LogTemp, Warning, TEXT("--------------------"))
	if (TargetPickup && ChosenPickupNeedValue > 0.0f)
		Blackboard->SetValueAsObject(TargetPickupKey.SelectedKeyName, TargetPickup);
	else
		Blackboard->SetValueAsObject(TargetPickupKey.SelectedKeyName, nullptr);
	Blackboard->SetValueAsFloat(ChosenPickupNeedValueKey.SelectedKeyName, ChosenPickupNeedValue);

	if (!bIsPickupMemoryTimerRun)
	{
		CyclePickupMemoryTimer();
		bIsPickupMemoryTimerRun = true;
	}
}


TArray<APickup*> UBTS_FindPickupToTake::GetAllPickupsInFieldOfView()
{
	TArray<AActor*> AllPickupRespawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickupRespawnPoint::StaticClass(), AllPickupRespawnPoints);
	TArray<APickup*> PickupsInFOV;
	FVector BotForwardVector = OwnerPawn->GetActorForwardVector();
	for (AActor* PickupRespawnPoint : AllPickupRespawnPoints)
	{
		APickup* Pickup = Cast<APickup>(Cast<APickupRespawnPoint>(PickupRespawnPoint)->GetPickupComponent()->GetChildActor());
		FVector BotVectorToPickup = (Pickup->GetActorLocation() - OwnerPawn->GetActorLocation()).GetSafeNormal();

		float AngleToPickup = UKismetMathLibrary::DegAcos(FVector::DotProduct(BotForwardVector, BotVectorToPickup));
		float DistanceToPickup = (Pickup->GetActorLocation() - OwnerPawn->GetActorLocation()).Size();
		if (AngleToPickup <= VisionHalfAngle && DistanceToPickup <= RangeOfVision && Pickup->IsPickupActive())
			PickupsInFOV.Emplace(Pickup);
	}
	return PickupsInFOV;
}

void UBTS_FindPickupToTake::FilterVisiblePickups(TArray<APickup*> PickupsInFOV)
{
	for (APickup* Pickup : PickupsInFOV)
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
	for (auto& Element : KnownPickups)
	{
		FPickupData PickupData = Element.Value;
		EPickupType PickupType = PickupData.PickupRef->GetPickupType();
		
		switch (PickupType)
		{
		case EPickupType::Health:
			CalculateNeedValueAndSetTargetPickup(PickupData, HealthNeedRatio);
			break;
		case EPickupType::Armor:
			CalculateNeedValueAndSetTargetPickup(PickupData, ArmorNeedRatio);
			break;
		case EPickupType::AssaultRifleAmmo:
			CalculateNeedValueAndSetTargetPickup(PickupData, AssaultRifleAmmoNeedRatio);
			break;
		case EPickupType::ShotgunAmmo:
			CalculateNeedValueAndSetTargetPickup(PickupData,ShotgunAmmoNeedRatio);
			break;
		case EPickupType::RocketLauncherAmmo:
			CalculateNeedValueAndSetTargetPickup(PickupData, RocketLauncherAmmoNeedRatio);
			break;
		case EPickupType::Weapon:
			CalculateWeaponNeedValue(PickupData);
			break;
		case EPickupType::Powerup:
			CalculateNeedValueAndSetTargetPickup(PickupData, PowerupNeedRatio);
			break;
		}
	}
}

void UBTS_FindPickupToTake::CalculateNeedValueAndSetTargetPickup(FPickupData PickupData, float NeedRatio)
{
	float PickupValue;
	PickupValue = (RangeOfVision / PickupData.DistanceToPickup) * static_cast<float>(PickupData.PickupRef->IsPickupActive()) * NeedRatio * PickupData.PickupRef->GetPickupValue();
	//UE_LOG(LogTemp, Warning, TEXT("%s --- %f ^^^ ChosenPickupNeedRatio --- %f"), *PickupData.PickupRef->GetName(), PickupValue, ChosenPickupNeedValue)
	if (PickupValue > ChosenPickupNeedValue)
	{
		ChosenPickupNeedValue = PickupValue;
		TargetPickup = PickupData.PickupRef;
	}
}

void UBTS_FindPickupToTake::CalculateWeaponNeedValue(FPickupData PickupData)
{
	switch (Cast<AWeapon>(PickupData.PickupRef)->GetWeaponType())
	{
	case(EWeaponType::AssaultRifle):
		CalculateNeedValueAndSetTargetPickup(PickupData, AssaultRifleNeedRatio);
		break;
	case(EWeaponType::Shotgun):
		CalculateNeedValueAndSetTargetPickup(PickupData, ShotgunNeedRatio);
		break;
	case(EWeaponType::RocketLauncher):
		CalculateNeedValueAndSetTargetPickup(PickupData, RocketLauncherNeedRatio);
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

void UBTS_FindPickupToTake::CalculateNeeds()
{
	CalculateHealthNeed();
	CalculateArmorNeed();
	CalculateAssaultRifleAmmoAndWeaponNeed();
	CalculateShotgunAmmoAndWeaponNeed();
	CalculateRocketLauncherAmmoAndWeaponNeed();
	CalculatePowerupNeed();
}

void UBTS_FindPickupToTake::CalculateHealthNeed()
{
	if (OwnerPawn->GetHealth() < 70)
		HealthNeedRatio = 2.0f;
	else if (OwnerPawn->GetHealth() >= 100)
		HealthNeedRatio = 0.0f;
	else
		HealthNeedRatio = 0.5f;
}

void UBTS_FindPickupToTake::CalculateArmorNeed()
{
	if (OwnerPawn->GetArmor() < 50)
		ArmorNeedRatio = 1.5f;
	else if (OwnerPawn->GetArmor() >= 100)
		ArmorNeedRatio = 0.0f;
	else
		ArmorNeedRatio = 0.5f;
}

void UBTS_FindPickupToTake::CalculateAssaultRifleAmmoAndWeaponNeed()
{
	AWeapon* AssaultRifleRef = OwnerPawn->GetWeaponByType(EWeaponType::AssaultRifle);
	if (AssaultRifleRef)
	{
		AssaultRifleAmmoNeedRatio = AssaultRifleRef->GetRelativeAmmoCapacity();
		AssaultRifleNeedRatio = AssaultRifleRef->GetRelativeAmmoCapacity();
	}
	else
	{
		AssaultRifleAmmoNeedRatio = 0.0f;
		AssaultRifleNeedRatio = 1.2f;
	}
}

void UBTS_FindPickupToTake::CalculateShotgunAmmoAndWeaponNeed()
{
	AWeapon* ShotgunRef = OwnerPawn->GetWeaponByType(EWeaponType::Shotgun);
	if (ShotgunRef)
	{
		ShotgunAmmoNeedRatio = ShotgunRef->GetRelativeAmmoCapacity();
		ShotgunNeedRatio = ShotgunRef->GetRelativeAmmoCapacity();
	}
	else
	{
		ShotgunAmmoNeedRatio = 0.0f;
		ShotgunNeedRatio = 1.2f;
	}
}

void UBTS_FindPickupToTake::CalculateRocketLauncherAmmoAndWeaponNeed()
{
	AWeapon* RocketLauncherRef = OwnerPawn->GetWeaponByType(EWeaponType::RocketLauncher);
	if (RocketLauncherRef)
	{
		RocketLauncherAmmoNeedRatio = RocketLauncherRef->GetRelativeAmmoCapacity();
		RocketLauncherNeedRatio = RocketLauncherRef->GetRelativeAmmoCapacity();
	}
	else
	{
		RocketLauncherAmmoNeedRatio = 0.0f;
		RocketLauncherNeedRatio = 1.2f;
	}
}

void UBTS_FindPickupToTake::CalculatePowerupNeed()
{
	if (OwnerPawn->IsPowerupEnabled())
		PowerupNeedRatio = 0.0f;
	else
		PowerupNeedRatio = 1.1f;
}