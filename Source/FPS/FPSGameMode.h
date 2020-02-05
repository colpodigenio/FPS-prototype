// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPS.h"
#include "FPSGameMode.generated.h"

UCLASS()
class FPS_API AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AFPSGameMode();
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EBotDifficulty::Type> BotDifficulty;

	UPROPERTY(BlueprintReadOnly)
	TArray<FName> PlayersNames;

	FName HumanPlayerName;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
