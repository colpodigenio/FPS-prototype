// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "FPS.h"
#include "PowerupPickup.generated.h"

class AFPSCharacter;

UCLASS()
class FPS_API APowerupPickup : public APickup
{
	GENERATED_BODY()

public:
	APowerupPickup();
	virtual bool TryApplyToCharacter(AFPSCharacter* Character) override;

private:

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	EPowerupType PowerupType;

	void ApplyPowerup(AFPSCharacter* Character, EPowerupType PowerupType);
};
