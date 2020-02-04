// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FPS.h"
#include "FPSProfileSave.generated.h"

UCLASS()
class FPS_API UFPSProfileSave : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	TMap<FName, FPlayerProfileData> PlayerProfileData;	
};
