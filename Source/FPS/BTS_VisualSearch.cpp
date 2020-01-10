// Fill out your copyright notice in the Description page of Project Settings.

#include "BTS_VisualSearch.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"
#include "WorldCollision.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "PickupRespawnPoint.h"

UBTS_VisualSearch::UBTS_VisualSearch()
{
	bNotifyBecomeRelevant = true;
	bCallTickOnSearchStart = true;
	bIsPickupMemoryTimerRun = false;
	PickupMemoryDuration = 20;
	RangeOfVision = 10000.0f;
	VisionHalfAngle = 50;
	ChosenPickupNeedValue = 0.0f;
	float DefaultEnemyMemoryTime = 5.0f;
	float EnemyMemoryTime = 5.0f;
}

void UBTS_VisualSearch::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	OwnerController = Cast<AAIController>(OwnerComp.GetAIOwner());
	OwnerPawn = Cast<AFPSCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	Blackboard = OwnerComp.GetBlackboardComponent();
}

void UBTS_VisualSearch::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (!OwnerPawn.Get() || !Blackboard.Get()) return; // it is needed because first tick starts before OnBecomeRelevant

	SetEnemyToAttack();
	FocusOnEnemyOrClear();

	if (!EnemyToAttack.Get())
	{
		EnemyMemoryTime -= DeltaSeconds;
		if (EnemyMemoryTime <= 0)
		{
			LastSeenEnemy = nullptr;
			EnemyMemoryTime = DefaultEnemyMemoryTime;
		}
	}
	else
		EnemyMemoryTime = DefaultEnemyMemoryTime;

	ChosenPickupNeedValue = 0.0f;

	CalculateNeeds();
	FilterVisiblePickups(GetAllPickupsInFieldOfView());
	SetTargetPickup();

	if (TargetPickup.Get() && ChosenPickupNeedValue > 0.0f)
		Blackboard->SetValueAsObject(TargetPickupKey.SelectedKeyName, TargetPickup.Get());
	else
		Blackboard->SetValueAsObject(TargetPickupKey.SelectedKeyName, nullptr);
	Blackboard->SetValueAsFloat(ChosenPickupNeedValueKey.SelectedKeyName, ChosenPickupNeedValue);

	if (!bIsPickupMemoryTimerRun)
	{
		CyclePickupMemoryTimer();
		bIsPickupMemoryTimerRun = true;
	}
}

TArray<AFPSCharacter*> UBTS_VisualSearch::GetAllVisibleEnemiesInFieldOfView()
{
	TArray<AActor*> AllEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFPSCharacter::StaticClass(), AllEnemies);
	TArray<AFPSCharacter*> EnemiesInFOV;
	FVector BotForwardVector = OwnerPawn->GetActorForwardVector();
	for (AActor* Enemy : AllEnemies)
	{
		FVector BotVectorToEnemy = (Enemy->GetActorLocation() - OwnerPawn->GetActorLocation()).GetSafeNormal();
		float AngleToEnemy = UKismetMathLibrary::DegAcos(FVector::DotProduct(BotForwardVector, BotVectorToEnemy));
		float DistanceToEnemy = (Enemy->GetActorLocation() - OwnerPawn->GetActorLocation()).Size();
		if (AngleToEnemy <= VisionHalfAngle && DistanceToEnemy <= RangeOfVision)
			EnemiesInFOV.Emplace(Cast<AFPSCharacter>(Enemy));
	}
	return EnemiesInFOV;
}

void UBTS_VisualSearch::SetEnemyToAttack()
{
	FilterClosestVisibleEnemy(GetAllVisibleEnemiesInFieldOfView());
	Blackboard->SetValueAsObject(EnemyToAttackKey.SelectedKeyName, EnemyToAttack.Get());
	Blackboard->SetValueAsObject(LastSeenEnemyKey.SelectedKeyName, LastSeenEnemy.Get());
}

void UBTS_VisualSearch::FocusOnEnemyOrClear()
{
	if (EnemyToAttack.IsValid())
		OwnerController->SetFocus(EnemyToAttack.Get());
	else
		OwnerController->ClearFocus(EAIFocusPriority::Gameplay);
}

void UBTS_VisualSearch::FilterClosestVisibleEnemy(TArray<AFPSCharacter*> EnemiesInFOV)
{
	EnemyToAttack = nullptr;
	float DistanceToTargetEnemy = RangeOfVision;
	for (AFPSCharacter* Enemy : EnemiesInFOV)
	{
		FHitResult HitRes;
		FVector StartPoint = OwnerPawn->GetActorLocation();
		FVector EndPoint = Enemy->GetActorLocation();
		float DistanceToEnemy = (StartPoint - EndPoint).Size();
		if (!GetWorld()->LineTraceSingleByChannel(HitRes, StartPoint, EndPoint, ECC_Visibility)) // checks if there is something between bot and enemy
		{
			if (DistanceToEnemy < DistanceToTargetEnemy)
			{
				DistanceToTargetEnemy = DistanceToEnemy;
				EnemyToAttack = Enemy;
				LastSeenEnemy = Enemy;
			}
		}
	}
	DistanceToTargetEnemy = RangeOfVision;
}

TArray<APickup*> UBTS_VisualSearch::GetAllPickupsInFieldOfView()
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

void UBTS_VisualSearch::FilterVisiblePickups(TArray<APickup*> PickupsInFOV)
{
	for (APickup* Pickup : PickupsInFOV)
	{
		FHitResult HitRes;
		FVector StartPoint = OwnerPawn->GetActorLocation();
		FVector EndPoint = Pickup->GetActorLocation();
		float DistanceToPickup = (StartPoint - EndPoint).Size();
		if (!GetWorld()->LineTraceSingleByChannel(HitRes, StartPoint, EndPoint, ECC_Visibility))
		{
			FPickupData PickupData = FPickupData(Pickup, DistanceToPickup, PickupMemoryDuration);
			KnownPickups.Add(Pickup->PickupID, PickupData);
		}
	}
}

void UBTS_VisualSearch::SetTargetPickup()
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
			CalculateNeedValueAndSetTargetPickup(PickupData, ShotgunAmmoNeedRatio);
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

void UBTS_VisualSearch::CalculateNeedValueAndSetTargetPickup(FPickupData PickupData, float NeedRatio)
{
	float PickupValue;
	PickupValue = (RangeOfVision / PickupData.DistanceToPickup) * static_cast<float>(PickupData.PickupRef->IsPickupActive()) * NeedRatio * PickupData.PickupRef->GetPickupValue();

	if (PickupValue > ChosenPickupNeedValue)
	{
		ChosenPickupNeedValue = PickupValue;
		TargetPickup = PickupData.PickupRef;
	}
}

void UBTS_VisualSearch::CalculateWeaponNeedValue(FPickupData PickupData)
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

void UBTS_VisualSearch::CyclePickupMemoryTimer()
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
	GetWorld()->GetTimerManager().SetTimer(MemoryTimer, this, &UBTS_VisualSearch::CyclePickupMemoryTimer, 1.0f);
}

void UBTS_VisualSearch::CalculateNeeds()
{
	CalculateHealthNeed();
	CalculateArmorNeed();
	CalculateAssaultRifleAmmoAndWeaponNeed();
	CalculateShotgunAmmoAndWeaponNeed();
	CalculateRocketLauncherAmmoAndWeaponNeed();
	CalculatePowerupNeed();
}

void UBTS_VisualSearch::CalculateHealthNeed()
{
	if (OwnerPawn->GetHealth() < 70)
		HealthNeedRatio = 2.0f;
	else if (OwnerPawn->GetHealth() >= 100)
		HealthNeedRatio = 0.0f;
	else
		HealthNeedRatio = 0.5f;
}

void UBTS_VisualSearch::CalculateArmorNeed()
{
	if (OwnerPawn->GetArmor() < 50)
		ArmorNeedRatio = 1.5f;
	else if (OwnerPawn->GetArmor() >= 100)
		ArmorNeedRatio = 0.0f;
	else
		ArmorNeedRatio = 0.5f;
}

void UBTS_VisualSearch::CalculateAssaultRifleAmmoAndWeaponNeed()
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

void UBTS_VisualSearch::CalculateShotgunAmmoAndWeaponNeed()
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

void UBTS_VisualSearch::CalculateRocketLauncherAmmoAndWeaponNeed()
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

void UBTS_VisualSearch::CalculatePowerupNeed()
{
	if (OwnerPawn->IsPowerupEnabled())
		PowerupNeedRatio = 0.0f;
	else
		PowerupNeedRatio = 1.1f;
}