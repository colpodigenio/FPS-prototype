// Fill out your copyright notice in the Description page of Project Settings.

#include "AssaultRifle.h"
#include "FPS.h"

AAssaultRifle::AAssaultRifle()
{
	WeaponType = EWeapon::AssaultRifle;
	AmmoTotalCapacity = 180;
	AmmoMagazineCapacity = 30;
	AmmoTotal = 30;
	AmmoInMagazine = 30;
	DamageAmount = 20;
	FireRate = 13.0f; // should be 13 shots/sec for m4 rifle
	ReloadTime = 3.0f;
}

void AAssaultRifle::Fire()
{
	LastShotTime = GetWorld()->TimeSeconds;
	AmmoInMagazine--;
	if (AmmoInMagazine <= 0)
		StopFire();
	UE_LOG(LogTemp, Warning, TEXT("%s has fired, ammo in magazine %i"), *GetName(), AmmoInMagazine)
}
