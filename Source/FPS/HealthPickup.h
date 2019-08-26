// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "HealthPickup.generated.h"

class AFPSCharacter;

UCLASS()
class FPS_API AHealthPickup : public APickup
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	int32 HealAmount;

public:

	AHealthPickup(const FObjectInitializer& ObjectInitializer);

	virtual bool TryApplyToCharacter(AFPSCharacter* Character) override;
};
