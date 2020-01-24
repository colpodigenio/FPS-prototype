// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FPS.h"
#include "FPSAIController.generated.h"

class UScoreHandlingComponent;

UCLASS()
class FPS_API AFPSAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	AFPSAIController();

	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn = true) override;
	FRotator AimingDeviation;
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE UScoreHandlingComponent* GetScoreHandlingComp() const { return ScoreHandlingComponent; };

protected:

	virtual void BeginPlay() override;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UScoreHandlingComponent* ScoreHandlingComponent;
	void SetRandomAimingDeviation();
};
