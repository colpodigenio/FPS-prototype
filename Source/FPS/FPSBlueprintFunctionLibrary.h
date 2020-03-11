// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FPSBlueprintFunctionLibrary.generated.h"

UCLASS()
class FPS_API UFPSBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable)
	static void LoadInputConfig(UObject* Obj, FString Filename);

	UFUNCTION(BlueprintPure)
	static float GetMouseSensitivity();
	UFUNCTION(BlueprintCallable)
	static void SetMouseSensitivity(float NewSensitivity);
};
