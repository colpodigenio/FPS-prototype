// Fill out your copyright notice in the Description page of Project Settings.

#include "Shotgun.h"
#include "FPS.h"
#include "Kismet/KismetMathLibrary.h"
#include "Projectile.h"

AShotgun::AShotgun()
{
	WeaponType = EWeapon::Shotgun;
	AmmoTotalCapacity = 48;
	AmmoMagazineCapacity = 6;
	AmmoTotal = 12;
	AmmoInMagazine = 6;
	DamageAmount = 5.0f;
	FireRate = 4.0f;
	ReloadTime = 2.5f;
	AmountOfPelletsInShell = 12;
}

void AShotgun::ShotProjectile()
{
	int32 i = AmountOfPelletsInShell;
	
	while (i > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%i"), i);
		FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(GetMesh()->GetSocketLocation(TEXT("Muzzle")), FVector(2.f));
		FVector SpawnDirection = GetMesh()->GetSocketRotation(TEXT("ProjectileSpawnPoint")).Vector();
		SpawnDirection = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(SpawnDirection.GetSafeNormal(), 15.f);
		FRotator SpawnRotation = SpawnDirection.GetSafeNormal().ToOrientationRotator();
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AProjectile>(ProjectileType, SpawnLocation, SpawnRotation, SpawnParams);
		i--;
	}
}
