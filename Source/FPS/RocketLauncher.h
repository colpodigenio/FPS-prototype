// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "RocketLauncher.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API ARocketLauncher : public AWeapon
{
	GENERATED_BODY()
	
public:

	ARocketLauncher();

	virtual void StartFire() override;
	virtual void Fire() override;
	
};