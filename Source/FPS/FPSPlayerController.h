// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerController.generated.h"

class AFPSCharacter;
class UScoreHandlingComponent;

UCLASS()
class FPS_API AFPSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AFPSPlayerController();
	FORCEINLINE UScoreHandlingComponent* GetScoreHandlingComp() const { return ScoreHandlingComponent; };
	void SpawnAndPossesSpectator();
	void StartRespawnTimer();

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BlueprintProtected = "true"))
	TSubclassOf<AFPSCharacter> PawnToSpawn;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UScoreHandlingComponent* ScoreHandlingComponent;
	void RespawnPlayer();
};
