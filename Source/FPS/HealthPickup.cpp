// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthPickup.h"
#include "FPSCharacter.h"
#include "Components/HealthComponent.h"

AHealthPickup::AHealthPickup(const FObjectInitializer& ObjectInitializer)
{
	HealAmount = 25.0f;
}

void AHealthPickup::ApplyToCharacter(AFPSCharacter* Character)
{
	Super::ApplyToCharacter(Character);

	Character->GetHealthComponent()->ApplyHeal(HealAmount);
}
