// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"




void AWeapon::Fire_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("%s Fires"), *GetName())
}

void AWeapon::Aim_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("%s Aims"), *GetName())
}

void AWeapon::Reload_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("%s Reloads"), *GetName())
}
