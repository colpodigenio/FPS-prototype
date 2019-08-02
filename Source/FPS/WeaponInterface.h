// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPS_API IWeaponInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WeaponInterface")
	void StartFire();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WeaponInterface")
	void Fire();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WeaponInterface")
	void StopFire();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WeaponInterface")
	void Aim();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WeaponInterface")
	void Reload();
	
};
