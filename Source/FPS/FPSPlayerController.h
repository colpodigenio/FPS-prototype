// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerController.generated.h"

class AFPSCharacter;
class UScoreHandlingComponent;
class UControllerComponentsContainer;

UCLASS()
class FPS_API AFPSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AFPSPlayerController();
	//FORCEINLINE UScoreHandlingComponent* GetScoreHandlingComp() const { return ScoreHandlingComponent; };
	FORCEINLINE UControllerComponentsContainer* GetControllerComponentsContainer() const { return ContrCompContainer; };
	void SpawnAndPossesSpectator();

private:
	
// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
// 	UScoreHandlingComponent* ScoreHandlingComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UControllerComponentsContainer* ContrCompContainer;
	
};
