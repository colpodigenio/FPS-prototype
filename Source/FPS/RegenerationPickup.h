// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerupPickup.h"
#include "RegenerationPickup.generated.h"

class AFPSCharacter;

UCLASS()
class FPS_API ARegenerationPickup : public APowerupPickup
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	int32 RegenerationDuration;

protected:

	void EnablePowerup() override;
};
