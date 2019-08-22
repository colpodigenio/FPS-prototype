// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerupPickup.h"
#include "DamageBoostPickup.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API ADamageBoostPickup : public APowerupPickup
{
	GENERATED_BODY()

public:

	virtual void ApplyToCharacter(AFPSCharacter* Character) override;
	
};
