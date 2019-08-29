// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ShotgunPellet.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AShotgunPellet : public AProjectile
{
	GENERATED_BODY()

	virtual FVector FindShotDirection() override;

public:
	AShotgunPellet();
};
