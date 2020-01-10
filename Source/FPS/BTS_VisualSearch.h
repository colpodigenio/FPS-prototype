// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_VisualSearch.generated.h"

class AFPSCharacter;
class APickup;

struct FPickupData
{
	FPickupData(APickup* PickupRef, float DistanceToPickup, int32 MemoryDuration)
		: PickupRef(PickupRef), DistanceToPickup(DistanceToPickup), MemoryDuration(MemoryDuration) {};

	APickup* PickupRef;
	float DistanceToPickup;
	int32 MemoryDuration;
};

UCLASS()
class FPS_API UBTS_VisualSearch : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTS_VisualSearch();

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector EnemyToAttackKey;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector LastSeenEnemyLocationKey;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TargetPickupKey;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector ChosenPickupNeedValueKey;

protected:

	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:

	TArray<AFPSCharacter*> GetAllVisibleEnemiesInFieldOfView();
	void SetEnemyToAttackOrLastSeenLocation();
	void FocusOnEnemyOrClear();
	void FilterClosestVisibleEnemy(TArray<AFPSCharacter*> EnemiesInFOV);

	TArray<APickup*> GetAllPickupsInFieldOfView();
	void FilterVisiblePickups(TArray<APickup*> PickupsInFOV);
	void SetTargetPickup();
	void CalculateNeedValueAndSetTargetPickup(FPickupData PickupData, float NeedRatio);
	void CalculateWeaponNeedValue(FPickupData PickupData);
	void CyclePickupMemoryTimer();
	void CalculateNeeds();
	void CalculateHealthNeed();
	void CalculateArmorNeed();
	void CalculateAssaultRifleAmmoAndWeaponNeed();
	void CalculateShotgunAmmoAndWeaponNeed();
	void CalculateRocketLauncherAmmoAndWeaponNeed();
	void CalculatePowerupNeed();


	TWeakObjectPtr<AAIController> OwnerController;
	TWeakObjectPtr<AFPSCharacter> OwnerPawn;
	TWeakObjectPtr<UBlackboardComponent> Blackboard;
	TWeakObjectPtr<AFPSCharacter> EnemyToAttack;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float RangeOfVision;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float VisionHalfAngle;
	FVector LastSeenEnemyLocation;
	
	TMap<int32, FPickupData> KnownPickups;
	TWeakObjectPtr<APickup> TargetPickup;

	float ChosenPickupNeedValue;
	float HealthNeedRatio;
	float ArmorNeedRatio;
	float AssaultRifleAmmoNeedRatio;
	float AssaultRifleNeedRatio;
	float ShotgunAmmoNeedRatio;
	float ShotgunNeedRatio;
	float RocketLauncherAmmoNeedRatio;
	float RocketLauncherNeedRatio;
	float PowerupNeedRatio;

	bool bIsPickupMemoryTimerRun;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	int32 PickupMemoryDuration;
};
