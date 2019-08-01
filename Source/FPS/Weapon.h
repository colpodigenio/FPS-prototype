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

	virtual void Fire_Implementation() override;
	virtual void Aim_Implementation() override;
	virtual void Reload_Implementation() override;
};
