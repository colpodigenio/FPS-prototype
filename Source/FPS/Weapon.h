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

public:

	AWeapon(const FObjectInitializer& ObjectInitializer);

	FOnTimelineFloat OnRecoilTimelineCallback;
	FOnTimelineEventStatic OnRecoilTimelineFinish;
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Fire"))
	virtual void StartFire();
	virtual void StopFire();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Reload"))
	virtual void StartReload();
	virtual bool TryApplyToCharacter(AFPSCharacter* Character) override;
	void AddAmmo(int32 AmountOfMagazines);
	void ShowMesh() override;
	void HideMesh() override;

	void ChangeDamage(int32 DamageBooster);
	bool CheckIfAmmoIsFull();
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; };
	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetAmmoTotal() const { return AmmoTotal; };
	FORCEINLINE float GetRelativeAmmoCapacity() const { return (AmmoTotal + AmmoInMagazine) / (AmmoTotalCapacity + AmmoMagazineCapacity); };
	UFUNCTION(BlueprintPure)
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; };
	FORCEINLINE int32 GetDamageBooster() const { return DamageBooster; };
	bool CheckIfAmmoRanOut();

protected:

	UPROPERTY(VisibleDefaultsOnly)
	int32 AmmoTotalCapacity; // total ammo that can be carried excluding ammo in weapon
	UPROPERTY(VisibleDefaultsOnly)
	int32 AmmoMagazineCapacity;
	UPROPERTY(VisibleAnywhere)
	int32 AmmoTotal; // ammo excluding ammo in magazine 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 AmmoInMagazine;
	int32 DamageAmount;
	int32 DefaultDamage;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	int32 DamageBooster;
	float FireRate; // shots amount in second
	UPROPERTY(BlueprintReadOnly)
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

private:

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

	virtual void Fire();
	UFUNCTION()
	void RecoilTimelineCallback(float Value);
	UFUNCTION()
	void RecoilTimelineFinish();
	void AddRecoil();
	void DecreaseAmmoAmount();
	void Reload();
};
