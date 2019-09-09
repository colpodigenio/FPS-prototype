// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthPickup.h"
#include "FPSCharacter.h"
#include "Components/HealthComponent.h"

AHealthPickup::AHealthPickup(const FObjectInitializer& ObjectInitializer)
{
	HealAmount = 25.0f;
	PickupType = EPickupType::Health;
}

bool AHealthPickup::TryApplyToCharacter(AFPSCharacter* Character)
{
	if (!Character) return false;
	if (Character->CheckIfHealthIsFull())
		return false;
	else
	{
		Character->GetHealthComponent()->ApplyHeal(HealAmount);
		return true;
	}
}
