#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_FindEnemyToAttack.generated.h"

class AAIController;
class AFPSCharacter;
class APickup;

UCLASS()
class FPS_API UBTS_FindEnemyToAttack : public UBTService
{
	GENERATED_BODY()

public:

	UBTS_FindEnemyToAttack();
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector EnemyToAttackKey;
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector LastSeenEnemyLocationKey;

private:

	TArray<AFPSCharacter*> GetAllVisibleEnemiesInFieldOfView();
	void SetEnemyToAttackOrLastSeenLocation();
	void FocusOnEnemyOrClear();
	AFPSCharacter* FilterClosestVisibleEnemy(TArray<AFPSCharacter*> EnemiesInFOV);

	APickup* TargetPickup;
	TWeakObjectPtr<AAIController> OwnerController;
	TWeakObjectPtr<AFPSCharacter> OwnerPawn;
	TWeakObjectPtr<UBlackboardComponent> Blackboard;
	TWeakObjectPtr<AFPSCharacter> EnemyToAttack;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float RangeOfVision;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float VisionHalfAngle;
	FVector LastSeenEnemyLocation;

protected:

	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
