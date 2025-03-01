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
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds); 

	if (!OwnerPawn.Get() || !Blackboard.Get()) return; // needed because first tick starts before OnBecomeRelevant
	float DistanceToEnemy = FindDistanceToEnemy();
		
	if (DistanceToEnemy > MinDistanceToUseAssaultRifle)
		TakeWeaponIfItIsNotEmpty(EWeaponType::AssaultRifle, EWeaponType::RocketLauncher, EWeaponType::Shotgun);
	else if (DistanceToEnemy > MinDistanceToUseRocketLauncher)
		TakeWeaponIfItIsNotEmpty(EWeaponType::RocketLauncher, EWeaponType::AssaultRifle, EWeaponType::Shotgun);
	else if (DistanceToEnemy <= MinDistanceToUseRocketLauncher)
		TakeWeaponIfItIsNotEmpty(EWeaponType::Shotgun, EWeaponType::RocketLauncher, EWeaponType::AssaultRifle);
	Blackboard->SetValueAsEnum(UsedWeaponKey.SelectedKeyName, (uint8)OwnerPawn->GetCurrentWeapon()->GetWeaponType());
}

float UBTS_ChooseWeapon::FindDistanceToEnemy()
{
	static float LastDistanceToEnemy = 2 * MinDistanceToUseAssaultRifle;
	AFPSCharacter* EnemyToAttack = Cast<AFPSCharacter>(Blackboard->GetValueAsObject(EnemyToAttackKey.SelectedKeyName));
	if (!EnemyToAttack) return LastDistanceToEnemy;
	float DistanceToEnemy = (EnemyToAttack->GetActorLocation() - OwnerPawn->GetActorLocation()).Size();
	LastDistanceToEnemy = DistanceToEnemy;
	return DistanceToEnemy;
}

void UBTS_ChooseWeapon::TakeWeaponIfItIsNotEmpty(EWeaponType FirstWeaponToTry, EWeaponType SecondWeaponToTry, EWeaponType ThirdWeaponToTry)
{
	if (OwnerPawn->CheckIfCharacterHasWeapon(FirstWeaponToTry) && !OwnerPawn->CheckIfWeaponByTypeIsEmpty(FirstWeaponToTry))
		OwnerPawn->TakeNewWeapon(FirstWeaponToTry);
	else if (OwnerPawn->CheckIfCharacterHasWeapon(SecondWeaponToTry) && !OwnerPawn->CheckIfWeaponByTypeIsEmpty(SecondWeaponToTry))
		OwnerPawn->TakeNewWeapon(SecondWeaponToTry);
	else if (OwnerPawn->CheckIfCharacterHasWeapon(ThirdWeaponToTry) && !OwnerPawn->CheckIfWeaponByTypeIsEmpty(ThirdWeaponToTry))
		OwnerPawn->TakeNewWeapon(ThirdWeaponToTry);
}

