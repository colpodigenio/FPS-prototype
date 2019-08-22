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

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	int32 AmountOfMagazinesToAdd;	

public:
	
	virtual void ApplyToCharacter(AFPSCharacter* Character) override;

	UPROPERTY(EditDefaultsOnly)
	EWeaponType AmmoType;
};
