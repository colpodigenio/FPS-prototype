// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "AssaultRifle.generated.h"

UCLASS()
class FPS_API AAssaultRifle : public AWeapon
{
	GENERATED_BODY()
	
public:

	AAssaultRifle(const FObjectInitializer& ObjectInitializer);
};
