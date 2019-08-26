// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoPickup.h"
#include "FPSCharacter.h"
#include "Weapon.h"

bool AAmmoPickup::TryApplyToCharacter(AFPSCharacter* Character)
{
	if (!Character) return false;
	checkf(WeaponType != EWeaponType::None, TEXT("Weapon type in ammo class is not set"))
	AWeapon* WeaponToAddAmmoTo = Character->GetWeaponByType(WeaponType);
	if (WeaponToAddAmmoTo->CheckIfAmmoIsFull())
		return false;
	else
	{
		WeaponToAddAmmoTo->AddAmmo(AmountOfMagazinesToAdd);
		return true;
	}
}
