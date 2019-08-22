// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "PowerupPickup.generated.h"

class AFPSCharacter;

UCLASS()
class FPS_API APowerupPickup : public APickup
{
	GENERATED_BODY()

	//Not needed for Regeneration powerup
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float PowerupDuration;

	void StartPowerupEffect();
	void EndPowerupEffect();

public:
	virtual void ApplyToCharacter(AFPSCharacter* Character) override;

protected:
	
	TWeakObjectPtr<AFPSCharacter> CharacterToApply;
	virtual void EnablePowerup();
	virtual void DisablePowerup();
	
};
