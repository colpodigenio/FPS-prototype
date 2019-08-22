// Fill out your copyright notice in the Description page of Project Settings.

#include "RegenerationPickup.h"
#include "FPSCharacter.h"
#include "Components/HealthComponent.h"

void ARegenerationPickup::EnablePowerup()
{
	Super::EnablePowerup();

	if(CharacterToApply.Get())
		CharacterToApply->GetHealthComponent()->StartRegenerateHealthTimer(RegenerationDuration);
}

