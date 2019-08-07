// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

class AFPSCharacter;

UCLASS()
class FPS_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	APickup();
	virtual void Tick(float DeltaTime) override;	
	virtual void AddThisToCharacterInventory(AFPSCharacter* Character);


protected:
	virtual void BeginPlay() override;	
	
};
