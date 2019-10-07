// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_ChooseWeapon.generated.h"

class AFPSCharacter;

UCLASS()
class FPS_API UBTS_ChooseWeapon : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTS_ChooseWeapon();
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector EnemyToAttackKey;
	UPROPERTY(EditAnywhere, Category = WeaponRanges)
	float MinDistanceToUseAssaultRifle;
	UPROPERTY(EditAnywhere, Category = WeaponRanges)
	float MinDistanceToUseRocketLauncher;


private:

	TWeakObjectPtr<AFPSCharacter> OwnerPawn;
	TWeakObjectPtr<UBlackboardComponent> Blackboard;

	float FindDistanceToEnemy();

protected:

	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;	
};
