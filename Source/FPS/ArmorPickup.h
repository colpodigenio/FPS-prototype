// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "ArmorPickup.generated.h"

class AFPSCharacter;

UCLASS()
class FPS_API AArmorPickup : public APickup
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	int32 ArmorAmount;

public:

	AArmorPickup(const FObjectInitializer& ObjectInitializer);

	virtual void ApplyToCharacter(AFPSCharacter* Character) override;
	
};