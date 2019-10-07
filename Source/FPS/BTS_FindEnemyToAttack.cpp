// Fill out your copyright notice in the Description page of Project Settings.

#include "BTS_FindEnemyToAttack.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"
#include "WorldCollision.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Weapon.h"
#include "Kismet/KismetMathLibrary.h"

UBTS_FindEnemyToAttack::UBTS_FindEnemyToAttack()
{
	bNotifyBecomeRelevant = true;
	bCallTickOnSearchStart = true;
	RangeOfVision = 10000.0f;
	VisionHalfAngle = 50;
}

void UBTS_FindEnemyToAttack::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	OwnerPawn = Cast<AFPSCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	Blackboard = OwnerComp.GetBlackboardComponent();
}

void UBTS_FindEnemyToAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (!OwnerPawn.Get() || !Blackboard.Get()) return;

	SetEnemyToAttackOrLastSeenLocation();
}


TArray<AFPSCharacter*> UBTS_FindEnemyToAttack::GetAllVisibleEnemiesInFieldOfView()
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

void UBTS_FindEnemyToAttack::SetEnemyToAttackOrLastSeenLocation()
{
	AFPSCharacter* EnemyToAttack = FilterClosestVisibleEnemy(GetAllVisibleEnemiesInFieldOfView());
	Blackboard->SetValueAsObject(EnemyToAttackKey.SelectedKeyName, EnemyToAttack);
	Blackboard->SetValueAsVector(LastSeenEnemyLocationKey.SelectedKeyName, LastSeenEnemyLocation);
}

AFPSCharacter* UBTS_FindEnemyToAttack::FilterClosestVisibleEnemy(TArray<AFPSCharacter*> EnemiesInFOV)
{
	AFPSCharacter* TargetEnemy = nullptr;
	float DistanceToTargetEnemy = RangeOfVision;
	for (AFPSCharacter* Enemy : EnemiesInFOV)
	{
		FHitResult HitRes;
		FVector StartPoint = OwnerPawn->GetActorLocation();
		FVector EndPoint = Enemy->GetActorLocation();
		float DistanceToEnemy = (StartPoint - EndPoint).Size();
		if (!GetWorld()->LineTraceSingleByChannel(HitRes, StartPoint, EndPoint, ECC_Visibility))
		{
			if (DistanceToEnemy < DistanceToTargetEnemy)
			{
				DistanceToTargetEnemy = DistanceToEnemy;
				LastSeenEnemyLocation = Enemy->GetActorLocation();
				TargetEnemy = Enemy;
			}
		}
	}
	DistanceToTargetEnemy = RangeOfVision;
	return TargetEnemy;
}