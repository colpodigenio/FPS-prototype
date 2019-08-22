// Fill out your copyright notice in the Description page of Project Settings.

#include "RocketLauncher.h"
#include "FPS.h"

ARocketLauncher::ARocketLauncher(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	WeaponType = EWeaponType::RocketLauncher;
	AmmoTotalCapacity = 10;
	AmmoMagazineCapacity = 1;
	AmmoTotal = 2;
	AmmoInMagazine = 1;
	DamageAmount = 100;
	FireRate = 0.1f; 
	ReloadTime = 1.5f;
	RecoilValue = -10.0f;
	DefaultRecoilValue = -10.0f;
}
