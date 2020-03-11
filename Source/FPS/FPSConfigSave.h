// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FPSConfigSave.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UFPSConfigSave : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite)
	float MouseSensitivity;

	// InputSave to load default inputs
	// Graphics config such as resolution, anti-aliasing, VSync, Texture quality and others
};
