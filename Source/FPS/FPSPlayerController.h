// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ScoreCountingInterface.h"
#include "FPSPlayerController.generated.h"

class AFPSCharacter;
UCLASS()
class FPS_API AFPSPlayerController : public APlayerController, public IScoreCountingInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void SetPlayerNameInGM_Implementation() override;

protected:
	virtual void BeginPlay() override;
};
