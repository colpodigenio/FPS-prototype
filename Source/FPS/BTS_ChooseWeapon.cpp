// Fill out your copyright notice in the Description page of Project Settings.

#include "BTS_ChooseWeapon.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "FPSCharacter.h"
#include "Weapon.h"

UBTS_ChooseWeapon::UBTS_ChooseWeapon()
{
	bNotifyBecomeRelevant = true;
	bCallTickOnSearchStart = true;
	MinDistanceToUseAssaultRifle = 800.0f;
	MinDistanceToUseRocketLauncher = 400.0f;
}

void UBTS_ChooseWeapon::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	OwnerPawn = Cast<AFPSCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	Blackboard = OwnerComp.GetBlackboardComponent();
}

void UBTS_ChooseWeapon::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds); // needed because first tick starts before OnBecomeRelevant

	if (!OwnerPawn.Get() || !Blackboard.Get()) return;
	float DistanceToEnemy = FindDistanceToEnemy();
		
	if (OwnerPawn->CheckIfCharacterHasWeapon(EWeaponType::AssaultRifle) && !OwnerPawn->CheckIfAssaultRifleIsEmpty() && DistanceToEnemy > MinDistanceToUseAssaultRifle)
	{
		OwnerPawn->TakeAssaultRifle();
	}
	else if (OwnerPawn->CheckIfCharacterHasWeapon(EWeaponType::RocketLauncher) && !OwnerPawn->CheckIfAssaultRifleIsEmpty() && DistanceToEnemy > MinDistanceToUseRocketLauncher)
	{
		OwnerPawn->TakeRocketLauncher();
	}
	else if (OwnerPawn->CheckIfCharacterHasWeapon(EWeaponType::Shotgun) && !OwnerPawn->CheckIfAssaultRifleIsEmpty() && DistanceToEnemy <= 400.0f)
	{
		OwnerPawn->TakeShotgun();
	}
}

float UBTS_ChooseWeapon::FindDistanceToEnemy()
{
	static float LastDistanceToEnemy = 1000.0f;
	AFPSCharacter* EnemyToAttack = Cast<AFPSCharacter>(Blackboard->GetValueAsObject(EnemyToAttackKey.SelectedKeyName));
	if (!EnemyToAttack) return LastDistanceToEnemy;
	float DistanceToEnemy = (EnemyToAttack->GetActorLocation() - OwnerPawn->GetActorLocation()).Size();
	LastDistanceToEnemy = DistanceToEnemy;
	return DistanceToEnemy;
}


