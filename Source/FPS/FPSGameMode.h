// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPS.h"
#include "FPSGameMode.generated.h"

class AAIController;

UCLASS()
class FPS_API AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AFPSGameMode();
	
	EBotDifficulty::Type BotDifficulty;

	UPROPERTY(BlueprintReadOnly)
	TArray<FString> PlayersNames;

	UPROPERTY(BlueprintReadOnly)
	FString HumanPlayerName;
	bool HasMatchPlayed() const { return bHasMatchPlayed; };

protected:
	virtual void BeginPlay() override;

	bool bHasMatchPlayed;

private:
	void SpawnPlayers();
	int32 NumberOfPlayers;
	UPROPERTY(EditAnywhere, Category = "Classes", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AAIController> AIControllerClass;
};
