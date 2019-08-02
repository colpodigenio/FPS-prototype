// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupRespawnPoint.generated.h"

UCLASS()
class FPS_API APickupRespawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	APickupRespawnPoint();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
};
