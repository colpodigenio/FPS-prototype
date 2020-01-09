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
#include "DrawDebugHelpers.h"

static int32 DebugConeDraw = 0;
FAutoConsoleVariableRef CVARDDrawDebugCone(
	TEXT("Draw.Debug"),
	DebugConeDraw,
	TEXT("Draws cone from bot to show where it looks."),
	ECVF_Cheat
);

UBTS_FindEnemyToAttack::UBTS_FindEnemyToAttack()
{
	bNotifyBecomeRelevant = true;
	bCallTickOnSearchStart = true;
	RangeOfVision = 10000.0f;
	VisionHalfAngle = 50;
	bIsMemoryTimerStarted = false;
	MemoryTime = 3.0f;
}

void UBTS_FindEnemyToAttack::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	OwnerController = Cast<AAIController>(OwnerComp.GetAIOwner());
	OwnerPawn = Cast<AFPSCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	Blackboard = OwnerComp.GetBlackboardComponent();
}

void UBTS_FindEnemyToAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (!OwnerPawn.Get() || !Blackboard.Get()) return; // it is needed because first tick starts before OnBecomeRelevant

	SetEnemyToAttackOrLastSeenLocation();
	FocusOnEnemyOrClear();
	if (bIsMemoryTimerStarted)
		MemoryTime -= DeltaSeconds;
	else
		MemoryTime = 3.0f;
	UE_LOG(LogTemp, Warning, TEXT("%i enemies in FOV"), GetAllVisibleEnemiesInFieldOfView().Num())
	for (auto It : GetAllVisibleEnemiesInFieldOfView())
	{
		UE_LOG(LogTemp,Warning,TEXT("%s"), *It->GetName())
	}
}



void UBTS_FindEnemyToAttack::SetEnemyToAttackOrLastSeenLocation()
{
	FilterClosestVisibleEnemy(GetAllVisibleEnemiesInFieldOfView());
	Blackboard->SetValueAsObject(EnemyToAttackKey.SelectedKeyName, EnemyToAttack.Get());
	Blackboard->SetValueAsVector(LastSeenEnemyLocationKey.SelectedKeyName, LastSeenEnemyLocation);
}

void UBTS_FindEnemyToAttack::FocusOnEnemyOrClear()
{
	if (EnemyToAttack.IsValid())
		OwnerController->SetFocus(EnemyToAttack.Get());
	else
		OwnerController->ClearFocus(EAIFocusPriority::Gameplay);
}

void UBTS_FindEnemyToAttack::FilterClosestVisibleEnemy(TArray<AFPSCharacter*> EnemiesInFOV)
{
	if (EnemiesInFOV.Num() > 0)
	{
		bIsMemoryTimerStarted = false;
		EnemyToAttack = nullptr;
		float DistanceToTargetEnemy = RangeOfVision;
		for (AFPSCharacter* Enemy : EnemiesInFOV)
		{
			FVector StartPoint = OwnerPawn->GetActorLocation();
			FVector EndPoint = Enemy->GetActorLocation();
			float DistanceToEnemy = (StartPoint - EndPoint).Size();
			if (DistanceToEnemy < DistanceToTargetEnemy)
			{
				DistanceToTargetEnemy = DistanceToEnemy;
				LastSeenEnemyLocation = Enemy->GetActorLocation();
				EnemyToAttack = Enemy;
			}
		}
		DistanceToTargetEnemy = RangeOfVision;
	}
	else
	{
		if (EnemyToAttack.Get())
		{
			bIsMemoryTimerStarted = true;
			if (MemoryTime <= 0.f)
				EnemyToAttack = nullptr;
		}
	}
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
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OwnerPawn.Get());
		if(GetWorld()->LineTraceSingleByChannel(HitResult, OwnerPawn->GetActorLocation(), Enemy->GetActorLocation(), ECC_Camera, QueryParams))
		DrawDebugLine(GetWorld(), OwnerPawn->GetActorLocation(), HitResult.ImpactPoint, FColor::Purple, false, 5.f);
		if (AngleToEnemy <= VisionHalfAngle && DistanceToEnemy <= RangeOfVision)
			if(Cast<AFPSCharacter>(HitResult.GetActor()))
				EnemiesInFOV.Emplace(Cast<AFPSCharacter>(Enemy));
	}
	return EnemiesInFOV;
}