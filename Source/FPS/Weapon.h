// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "FPS.h"
#include "Weapon.generated.h"

class AProjectile;
class UParticleSystemComponent;

UCLASS()
class FPS_API AWeapon : public APickup
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* MuzzleFlashEffect;

protected:

	int32 AmmoTotalCapacity;
	int32 AmmoMagazineCapacity;
	int32 AmmoTotal; // ammo excluding ammo in magazine 
	int32 AmmoInMagazine;
	int32 DamageAmount;
	//float RecoilValue;
	float FireRate; // shots in second
	float ReloadTime; // seconds
	float LastShotTime;
	bool bIsReloading;
	bool bFirstShotFired;
	EWeapon WeaponType;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectile> ProjectileType;

	FTimerHandle FireTimer;

	virtual void Fire();
	virtual void ShotProjectile();
	void DecreaseAmmoAmount();

public:

	AWeapon();

	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return Mesh; };
	void AddAmmo();
	virtual void StartFire();
	virtual void StopFire();
	virtual void Aim();
	virtual void StartReload();
	virtual void Reload();
	virtual void AddThisToCharacterInventory(AFPSCharacter* Character) override;
};
