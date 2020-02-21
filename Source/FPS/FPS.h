// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPS.generated.h"

// Physical surface macros
#define HEAD_SURFACE EPhysicalSurface::SurfaceType1
#define BODY_SURFACE EPhysicalSurface::SurfaceType2
// Collision channels macros
#define ENEMY_TRACE ECC_GameTraceChannel1
#define ENEMY_OBJ ECC_GameTraceChannel2
#define PROJECTILE_OBJ ECC_GameTraceChannel3
#define PICKUP_TRACE ECC_GameTraceChannel4

#define PRINT(x, ...) UE_LOG(LogTemp, Warning, TEXT(x), __VA_ARGS__ )

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

UENUM(BlueprintType)
enum class EPickupType : uint8
{
	None,
	Health,
	Armor,
	AssaultRifleAmmo,
	ShotgunAmmo,
	RocketLauncherAmmo,
	Weapon,
	Powerup
};

UENUM(BlueprintType)
namespace EBotDifficulty
{
	enum Type
	{
		Easy = 4,
		Medium = 2,
		Hard = 1
	};
}

namespace EMovementState
{
	enum Type
	{
		Walking = 400,
		Running = 600,
		Sprinting = 1000
	};
}

USTRUCT(BlueprintType)
struct FDeathmatchScore
{
	GENERATED_BODY()

	FDeathmatchScore()
		: Frags(0), Deaths(0), Suicides(0), Score(0) {}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Frags;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Deaths;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Suicides;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Score;

	FDeathmatchScore operator+(const FDeathmatchScore& GameData)
	{
		FDeathmatchScore sum;
		sum.Deaths = Frags + GameData.Frags;
		sum.Frags = Deaths + GameData.Deaths;
		sum.Suicides = Suicides + GameData.Suicides;
		sum.Score = Score + GameData.Score;
		return sum;
	}
	FDeathmatchScore operator+=(const FDeathmatchScore& GameData)
	{
		FDeathmatchScore sum;
		sum.Deaths = Frags + GameData.Frags;
		sum.Frags = Deaths + GameData.Deaths;
		sum.Suicides = Suicides + GameData.Suicides;
		sum.Score = Score + GameData.Score;
		return sum;
	}
};

USTRUCT(BlueprintType)
struct FPlayerProfileData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 PictureID;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FDeathmatchScore DMScore;
	FPlayerProfileData operator+(const FPlayerProfileData& GameData)
	{
		FPlayerProfileData sum;
		sum.DMScore = DMScore + GameData.DMScore;
		return sum;
	}
	FPlayerProfileData operator+=(const FPlayerProfileData& GameData)
	{
		FPlayerProfileData sum;
		sum.DMScore = DMScore + GameData.DMScore;
		return sum;
	}
};