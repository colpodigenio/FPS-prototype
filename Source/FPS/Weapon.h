// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "FPS.h"
#include "Components/TimelineComponent.h"
#include "Weapon.generated.h"

class AProjectile;
class UParticleSystemComponent;
class UAudioComponent;
class UCurveFloat;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTimelineComponent* WeaponTimeline;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* RecoilCurve;

	UFUNCTION()
	void RecoilTimelineCallback(float Value);
	UFUNCTION()
	void RecoilTimelineFinish();
	void AddRecoil();
	virtual void Fire();
	void DecreaseAmmoAmount();
	void Reload();

protected:

	int32 AmmoTotalCapacity; // total ammo that can be carried excluding ammo in weapon
	int32 AmmoMagazineCapacity;
	int32 AmmoTotal; // ammo excluding ammo in magazine 
	int32 AmmoInMagazine;
	int32 DamageAmount;
	int32 DefaultDamage;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	int32 DamageBooster;
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
	virtual void BeginPlay() override;

public:
	
	AWeapon(const FObjectInitializer& ObjectInitializer);

	FOnTimelineFloat OnRecoilTimelineCallback;
	FOnTimelineEventStatic OnRecoilTimelineFinish;
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; };
	void AddAmmo(int32 AmountOfMagazines);
	virtual void StartFire();
	virtual void StopFire();
	virtual void Aim();
	virtual void StartReload();
	virtual bool TryApplyToCharacter(AFPSCharacter* Character) override;
	bool CheckIfAmmoIsFull();
	void ShowWeapon();
	void HideWeapon();
	FORCEINLINE int32 GetDamageBooster() const { return DamageBooster; };
	void ChangeDamage(int32 DamageBooster);
};
