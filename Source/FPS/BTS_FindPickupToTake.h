// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_FindPickupToTake.generated.h"

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
class FPS_API UBTS_FindPickupToTake : public UBTService
{
	GENERATED_BODY()

public:

	UBTS_FindPickupToTake();
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TargetPickupKey;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector ChosenPickupNeedValueKey;


private:

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

	TMap<int32, FPickupData> KnownPickups;
	APickup* TargetPickup;
	TWeakObjectPtr<AFPSCharacter> OwnerPawn;
	TWeakObjectPtr<UBlackboardComponent> Blackboard;

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
	int32 MemoryDuration;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float RangeOfVision;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float VisionHalfAngle;

protected:

	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
