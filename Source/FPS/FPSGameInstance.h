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
	TMap<FName, FPlayerProfileData> PlayersProfileData;
	virtual void Init() override;
	UPROPERTY(BlueprintReadWrite)
	FName ChosenPlayerProfile;
};
