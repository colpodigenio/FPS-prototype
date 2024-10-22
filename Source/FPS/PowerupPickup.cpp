// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerupPickup.h"
#include "FPSCharacter.h"
#include "FPS.h"
#include "Components/PowerupComponent.h"

APowerupPickup::APowerupPickup()
{
	PrimaryActorTick.bCanEverTick = false;

	PickupType = EPickupType::Powerup;
}

bool APowerupPickup::TryApplyToCharacter(AFPSCharacter* Character)
{

	if (Character->IsPowerupEnabled())
	{
		return false;
	}
	else
	{
		ApplyPowerup(Character, PowerupType);
		return true;
	}
}

void APowerupPickup::ApplyPowerup(AFPSCharacter* Character, EPowerupType PowerupType)
{
	Character->EnablePowerup(PowerupType);
}
