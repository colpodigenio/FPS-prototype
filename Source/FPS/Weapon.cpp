// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "UnrealMathUtility.h"
#include "FPSCharacter.h"
#include "Projectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

AWeapon::AWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.DoNotCreateDefaultSubobject(AWeapon::MeshComponentName))
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = WeaponMesh;
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MuzzleFlashEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MuzzleFlashEffect"));
	MuzzleFlashEffect->SetupAttachment(GetWeaponMesh(), TEXT("Muzzle"));
	MuzzleFlashEffect->SetAutoActivate(false);
	ShotSoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("ShotSoundEffect"));
	ShotSoundEffect->SetupAttachment(GetWeaponMesh(), TEXT("Muzzle"));
	ShotSoundEffect->SetAutoActivate(false);

	bIsReloading = false;
	bFirstShotFired = false;
	WeaponType = EWeaponType::None;
}

void AWeapon::AddAmmo(int32 AmountOfMagazines)
{
	int32 NewAmmoTotal = AmmoTotal + AmountOfMagazines * AmmoMagazineCapacity;
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

void AWeapon::AddRecoil()
{
	AFPSCharacter* Player = Cast<AFPSCharacter>(GetOwner());
	Player->AddControllerPitchInput(RecoilValue);
	UE_LOG(LogTemp, Warning, TEXT("Recoil Value = %f"), RecoilValue)
}

void AWeapon::Fire()
{
	LastShotTime = GetWorld()->TimeSeconds;
	ShotProjectile();
	AddRecoil();
	if (MuzzleFlashEffect)
		MuzzleFlashEffect->ActivateSystem();
	if (ShotSoundEffect)
		ShotSoundEffect->Activate(true);
	DecreaseAmmoAmount();
	UE_LOG(LogTemp, Warning, TEXT("%s has fired, ammo in magazine %i"), *GetName(), AmmoInMagazine)
}

void AWeapon::ShotProjectile()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = this;
	GetWorld()->SpawnActor<AProjectile>(ProjectileType, GetWeaponMesh()->GetSocketTransform(TEXT("Muzzle")), SpawnParams);
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

void AWeapon::ApplyToCharacter(AFPSCharacter* Character)
{
	Super::ApplyToCharacter(Character);
	if (Character->CheckIfCharacterHasWeapon(WeaponType))
		Character->AddAmmoFromWeaponPickup(WeaponType);
	else
		Character->AddWeaponFromWeaponPickup(WeaponType);
}

void AWeapon::ShowWeapon()
{
	GetWeaponMesh()->SetVisibility(true);
}

void AWeapon::HideWeapon()
{
	GetWeaponMesh()->SetVisibility(false);
}
