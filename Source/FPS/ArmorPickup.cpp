// Fill out your copyright notice in the Description page of Project Settings.

#include "ArmorPickup.h"
#include "FPSCharacter.h"
#include "Components/HealthComponent.h"

AArmorPickup::AArmorPickup(const FObjectInitializer& ObjectInitializer)
{
	ArmorAmount = 25.0f;
}

void AArmorPickup::ApplyToCharacter(AFPSCharacter* Character)
{
	Super::ApplyToCharacter(Character);

	Character->GetHealthComponent()->AddArmor(ArmorAmount);
}
