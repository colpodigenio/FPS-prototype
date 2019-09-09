// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CalculatePickupNeeds.generated.h"

class AFPSCharacter;

UCLASS()
class FPS_API UBTS_CalculatePickupNeeds : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTS_CalculatePickupNeeds();

	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector HealthNeedRatio;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector ArmorNeedRatio;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector	AssaultRifleAmmoNeedRatio;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector	ShotgunAmmoNeedRatio;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector	RocketLauncherAmmoNeedRatio;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector	AssaultRifleNeedRatio;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector	ShotgunNeedRatio;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector	RocketLauncherNeedRatio;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector	PowerupNeedRatio;

protected:

	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	TWeakObjectPtr<AFPSCharacter> OwnerPawn;
	TWeakObjectPtr<UBlackboardComponent> Blackboard;
	void CalculateHealthNeed();
	void CalculateArmorNeed();	
	void CalculateAssaultRifleAmmoAndWeaponNeed();
	void CalculateShotgunAmmoAndWeaponNeed();
	void CalculateRocketLauncherAmmoAndWeaponNeed();
	void CalculatePowerupNeed();
};
