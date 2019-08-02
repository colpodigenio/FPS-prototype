// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Weapon.generated.h"

UCLASS()
class FPS_API AWeapon : public APickup
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;

protected:

	int32 AmmoCapacity;
	int32 AmmoMagazineCapacity;
	int32 AmmoTotal; // ammo excluding ammo in magazine 
	int32 AmmoInMagazine;
	int32 DamageAmount;
	//flost RecoilValue;
	float FireRate; // shots in second
	float ReloadTime; // seconds
	float LastShotTime;
	bool bIsReloading;
	bool bFirstShotFired;

	FTimerHandle FireTimer;

	virtual void Fire();

public:

	AWeapon();

	virtual void StartFire();
	virtual void StopFire();
	virtual void Aim();
	virtual void StartReload();
	virtual void Reload();
};
