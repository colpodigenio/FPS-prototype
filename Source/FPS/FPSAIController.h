// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FPS.h"
#include "ScoreCountingInterface.h"
#include "FPSAIController.generated.h"

UCLASS()
class FPS_API AFPSAIController : public AAIController, public IScoreCountingInterface
{
	GENERATED_BODY()
	
public:

	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn = true) override;
	FRotator AimingDeviation;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void SetPlayerNameInGM_Implementation() override;

protected:

	virtual void BeginPlay() override;
	
private:

	void SetRandomAimingDeviation();
};
