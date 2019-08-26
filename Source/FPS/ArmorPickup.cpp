// Fill out your copyright notice in the Description page of Project Settings.

#include "ArmorPickup.h"
#include "FPSCharacter.h"
#include "Components/HealthComponent.h"

AArmorPickup::AArmorPickup(const FObjectInitializer& ObjectInitializer)
{
	ArmorAmount = 25.0f;
}

bool AArmorPickup::TryApplyToCharacter(AFPSCharacter* Character)
{
	if (!Character) return false;
	if (Character->CheckiIfArmorIsFull())
		return false;
	else
	{
		Character->GetHealthComponent()->AddArmor(ArmorAmount);
		return true;
	}
}
