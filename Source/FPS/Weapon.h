// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "WeaponInterface.h"
#include "Weapon.generated.h"

UCLASS()
class FPS_API AWeapon : public APickup, public IWeaponInterface
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
	virtual void Fire() override;
	UFUNCTION()
	virtual void Aim() override;
	UFUNCTION()
	virtual void Reload() override;
};
