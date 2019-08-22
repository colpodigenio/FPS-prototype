// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "FPS.h"
#include "Weapon.generated.h"

class AProjectile;
class UParticleSystemComponent;
class UAudioComponent;

UCLASS()
class FPS_API AWeapon : public APickup
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* MuzzleFlashEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* ShotSoundEffect;

	void AddRecoil();
	virtual void Fire();
	void DecreaseAmmoAmount();
	void Reload();

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
	float RecoilValue;
	float DefaultRecoilValue;
	bool bIsReloading;
	bool bFirstShotFired;
	EWeaponType WeaponType;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectile> ProjectileType;

	FTimerHandle FireTimer;

	virtual void ShotProjectile();

public:

	AWeapon(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; };
	void AddAmmo(int32 AmountOfMagazines);
	virtual void StartFire();
	virtual void StopFire();
	virtual void Aim();
	virtual void StartReload();
	virtual void ApplyToCharacter(AFPSCharacter* Character) override;
	void ShowWeapon();
	void HideWeapon();
};
