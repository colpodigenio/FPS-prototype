// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define Head EPhysicalSurface::SurfaceType1
#define Body EPhysicalSurface::SurfaceType2

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None,
	AssaultRifle,
	Shotgun,
	RocketLauncher
};

UENUM(BlueprintType)
enum class EPowerupType : uint8
{
	None,
	SpeedBoost,
	DamageBoost,
	HealthRegeneration
};