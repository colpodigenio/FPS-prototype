// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_FindPickupToTake.generated.h"

class AFPSCharacter;
class APickup;

struct FPickupData
{
	FPickupData(APickup* PickupRef, bool IsPickupActive, float DistanceToPickup, int32 MemoryDuration)
		: PickupRef(PickupRef), bIsPickupActive(IsPickupActive), DistanceToPickup(DistanceToPickup), MemoryDuration(MemoryDuration) {};

	APickup* PickupRef;
	bool bIsPickupActive;
	float DistanceToPickup;
	int32 MemoryDuration;
};

UCLASS()
class FPS_API UBTS_FindPickupToTake : public UBTService
{
	GENERATED_BODY()

public:

	UBTS_FindPickupToTake();
	UPROPERTY(EditAnywhere)
	int32 MemoryDuration;
	UPROPERTY(EditAnywhere)
	float RangeOfVision;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TargetPickupKey;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector HealthNeedRatioKey;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector ArmorNeedRatioKey;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector AssaultRifleAmmoNeedRatioKey;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector ShotgunAmmoNeedRatioKey;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector RocketLauncherAmmoNeedRatioKey;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector	AssaultRifleNeedRatioKey;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector	ShotgunNeedRatioKey;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector	RocketLauncherNeedRatioKey;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector	PowerupNeedRatioKey;

private:

	TArray<FHitResult> GetAllVisibleActorsExceptCharacters();
	void AddVisiblePickupsToKnown(TArray<FHitResult> VisibleActors);
	void SetTargetPickup();
	void CalculateNeedValueAndSetTargetPickup(FPickupData PickupData, float &LastPickupValue, float NeedRatio);
	void CalculateWeaponNeedValue(FPickupData PickupData, float &LastPickupValue);
	void CyclePickupMemoryTimer();

	TMap<int32, FPickupData> KnownPickups;
	APickup* TargetPickup;
	TWeakObjectPtr<AFPSCharacter> OwnerPawn;
	TWeakObjectPtr<UBlackboardComponent> Blackboard;

	bool bIsPickupMemoryTimerRun;

protected:

	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
