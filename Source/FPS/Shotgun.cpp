// Fill out your copyright notice in the Description page of Project Settings.

#include "Shotgun.h"
#include "FPS.h"
#include "Kismet/KismetMathLibrary.h"
#include "Projectile.h"
#include "FPSCharacter.h"

AShotgun::AShotgun()
	: Super(FObjectInitializer::Get())
{
	WeaponType = EWeaponType::Shotgun;
	AmmoTotalCapacity = 48;
	AmmoMagazineCapacity = 6;
	AmmoTotal = 12;
	AmmoInMagazine = 6;
	DamageAmount = 5.0f;
	FireRate = 4.0f;
	ReloadTime = 2.5f;
	AmountOfPelletsInShell = 12;
	RecoilValue = -2.0f;
	DefaultRecoilValue = -2.0f;
}

void AShotgun::ShotProjectile()
{
	int32 i = AmountOfPelletsInShell;
	
	while (i > 0)
	{
		FVector SpawnLocation = Cast<AFPSCharacter>(GetOwner())->GetFPSCameraLocation() + 50.0f * Cast<AFPSCharacter>(GetOwner())->GetFPSCameraForwardVector();//UKismetMathLibrary::RandomPointInBoundingBox(GetWeaponMesh()->GetSocketLocation(TEXT("Muzzle")), FVector(2.f));
		FVector SpawnDirection = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(Cast<AFPSCharacter>(GetOwner())->GetFPSCameraForwardVector(), 5.f);
		FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, SpawnLocation + SpawnDirection);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = this;
		GetWorld()->SpawnActor<AProjectile>(ProjectileType, SpawnLocation, SpawnRotation, SpawnParams);
		i--;
	}
}
