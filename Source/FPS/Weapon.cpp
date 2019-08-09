// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "UnrealMathUtility.h"
#include "FPSCharacter.h"
#include "Projectile.h"
#include "Particles/ParticleSystemComponent.h"

AWeapon::AWeapon()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MuzzleFlashEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MuzzleFlashEffect"));

	bIsReloading = false;
	bFirstShotFired = false;
	WeaponType = EWeapon::None;
}

void AWeapon::AddAmmo()
{
	int32 NewAmmoTotal = AmmoTotal + 2 * AmmoMagazineCapacity;
	if (NewAmmoTotal > AmmoTotalCapacity)
		AmmoTotal = AmmoTotalCapacity;
	else
		AmmoTotal = NewAmmoTotal;
}

void AWeapon::StartFire()
{
	if ((AmmoTotal <= 0 && AmmoInMagazine <=0) || bIsReloading) return;
	if (AmmoInMagazine <= 0)
	{
		StartReload();
		return;
	}
	float ShotDelay;
	if (bFirstShotFired)
		ShotDelay = FMath::Max(1 / FireRate - (GetWorld()->TimeSeconds - LastShotTime), 0.0f);
	else
		ShotDelay = 0.0f;
	bFirstShotFired = true;
	GetWorldTimerManager().SetTimer(FireTimer, this, &AWeapon::Fire, 1 / FireRate, true, ShotDelay);
}

void AWeapon::Fire()
{
	LastShotTime = GetWorld()->TimeSeconds;
	ShotProjectile();
	DecreaseAmmoAmount();
	UE_LOG(LogTemp, Warning, TEXT("%s has fired, ammo in magazine %i"), *GetName(), AmmoInMagazine)
}

void AWeapon::ShotProjectile()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AProjectile>(ProjectileType, GetMesh()->GetSocketTransform(TEXT("ProjectileSpawnPoint")), SpawnParams);
}

void AWeapon::DecreaseAmmoAmount()
{
	AmmoInMagazine--;
	if (AmmoInMagazine <= 0)
		StopFire();
}

void AWeapon::StopFire()
{
	if(GetWorldTimerManager().IsTimerActive(FireTimer))
		GetWorldTimerManager().ClearTimer(FireTimer);
}

void AWeapon::Aim()
{
	UE_LOG(LogTemp, Warning, TEXT("%s Aims"), *GetName())
}

void AWeapon::StartReload()
{
	if (bIsReloading || AmmoTotal == 0 || AmmoInMagazine == AmmoMagazineCapacity) return;
	FTimerHandle ReloadTimer;
	GetWorldTimerManager().SetTimer(ReloadTimer, this, &AWeapon::Reload, ReloadTime, false);
	bIsReloading = true;
}

void AWeapon::Reload()
{
	int32 DeltaAmmoInMagazine = AmmoMagazineCapacity - AmmoInMagazine;
	int32 NewAmmoTotal = AmmoTotal - DeltaAmmoInMagazine;
	if (NewAmmoTotal < 0)
	{
		AmmoInMagazine += DeltaAmmoInMagazine + NewAmmoTotal;
		AmmoTotal = 0;
	}
	else
	{
		AmmoInMagazine = AmmoMagazineCapacity;
		AmmoTotal -= DeltaAmmoInMagazine;
	}
	UE_LOG(LogTemp, Warning, TEXT("%s has been reloaded. %i bullets in magazine %i bullets total"), *GetName(), AmmoInMagazine, AmmoTotal)
	bIsReloading = false;
}

void AWeapon::AddThisToCharacterInventory(AFPSCharacter* Character)
{
	Super::AddThisToCharacterInventory(Character);
	if (Character->CheckIfCharacterHasWeapon(WeaponType))
		Character->AddAmmoFromWeaponPickup(WeaponType);
	else
		Character->AddWeaponFromWeaponPickup(WeaponType);
}
