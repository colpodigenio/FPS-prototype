// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerupPickup.h"
#include "SpeedBoostPickup.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API ASpeedBoostPickup : public APowerupPickup
{
	GENERATED_BODY()	

public:

	virtual void ApplyToCharacter(AFPSCharacter* Character) override;
};
