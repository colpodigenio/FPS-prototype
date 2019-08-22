// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerupPickup.h"
#include "FPSCharacter.h"

void APowerupPickup::ApplyToCharacter(AFPSCharacter* Character)
{
	Super::ApplyToCharacter(Character);

	CharacterToApply = Character;

	if (!CharacterToApply->bHasPowerup)
	{
		StartPowerupEffect();
	}
}

void APowerupPickup::EnablePowerup()
{
	CharacterToApply->bHasPowerup = true;
}

void APowerupPickup::DisablePowerup()
{
	CharacterToApply->bHasPowerup = false;
}

void APowerupPickup::StartPowerupEffect()
{
	EnablePowerup();
	FTimerHandle PowerupTimer;
	GetWorldTimerManager().SetTimer(PowerupTimer, this, &APowerupPickup::EndPowerupEffect, PowerupDuration, false);
}

void APowerupPickup::EndPowerupEffect()
{
	DisablePowerup();
}
