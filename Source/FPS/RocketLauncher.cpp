// Fill out your copyright notice in the Description page of Project Settings.

#include "RocketLauncher.h"
#include "FPS.h"

ARocketLauncher::ARocketLauncher()
	: Super(FObjectInitializer::Get())
{
	WeaponType = EWeaponType::RocketLauncher;
	AmmoTotalCapacity = 10;
	AmmoMagazineCapacity = 1;
	AmmoTotal = 2;
	AmmoInMagazine = 1;
	DamageAmount = 100;
	ReloadTime = 1.5f;
	FireRate = 1 / ReloadTime; 
	RecoilValue = -10.0f;
	DefaultRecoilValue = -10.0f;
}
