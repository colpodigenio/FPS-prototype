#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_FindEnemyToAttack.generated.h"

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
	AFPSCharacter* FilterClosestVisibleEnemy(TArray<AFPSCharacter*> EnemiesInFOV);

	APickup* TargetPickup;
	TWeakObjectPtr<AFPSCharacter> OwnerPawn;
	TWeakObjectPtr<UBlackboardComponent> Blackboard;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float RangeOfVision;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float VisionHalfAngle;
	FVector LastSeenEnemyLocation;

protected:

	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
