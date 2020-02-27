// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FPS.h"
#include "FPSGameInstance.generated.h"

class UFPSProfileSave;

UCLASS()
class FPS_API UFPSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TMap<FString, FPlayerProfileData> PlayersProfileData;
	virtual void Init() override;
	UPROPERTY(BlueprintReadWrite)
	FString ChosenPlayerName;
	EBotDifficulty::Type GetDifficulty() const { return BotDifficulty; };
	int32 GetNumberOfPlayers() const { return NumberOfPlayers; };

private:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FString, FMapData> MapsList;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EBotDifficulty::Type> BotDifficulty;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 NumberOfPlayers;
};
