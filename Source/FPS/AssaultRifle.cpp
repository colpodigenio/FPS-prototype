// Fill out your copyright notice in the Description page of Project Settings.

#include "AssaultRifle.h"
#include "FPS.h"
#include "Projectile.h"

AAssaultRifle::AAssaultRifle()
	: Super(FObjectInitializer::Get())
{
	WeaponType = EWeaponType::AssaultRifle;
	AmmoTotalCapacity = 180;
	AmmoMagazineCapacity = 30;
	AmmoTotal = 30;
	AmmoInMagazine = 30;
	DamageAmount = 20;
	FireRate = 13.0f; // should be 13 shots/sec for m4 rifle
	ReloadTime = 3.0f;
	RecoilValue = -0.5f;
	DefaultRecoilValue = -0.5f;
}

