// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoPickup.h"
#include "FPSCharacter.h"
#include "Weapon.h"

void AAmmoPickup::ApplyToCharacter(AFPSCharacter* Character)
{
	Super::ApplyToCharacter(Character);

	AWeapon* WeaponToAddAmmoTo = Character->GetWeaponByType(AmmoType);
	if (!WeaponToAddAmmoTo)
		return;
	WeaponToAddAmmoTo->AddAmmo(AmountOfMagazinesToAdd);
}
