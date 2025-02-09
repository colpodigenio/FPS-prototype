// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "FPS.h"
#include "AmmoPickup.generated.h"

class AFPSCharacter;

UCLASS()
class FPS_API AAmmoPickup : public APickup
{
	GENERATED_BODY()

public:

	virtual bool TryApplyToCharacter(AFPSCharacter* Character) override;

	UPROPERTY(EditDefaultsOnly)
	EWeaponType WeaponType;

private:

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	int32 AmountOfMagazinesToAdd;	
};
