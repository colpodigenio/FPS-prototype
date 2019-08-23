// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AShotgun : public AWeapon
{
	GENERATED_BODY()

	int32 AmountOfPelletsInShell;
	
public:
	
	AShotgun(const FObjectInitializer& ObjectInitializer);
	virtual void ShotProjectile() override;
	
};
