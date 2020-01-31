// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RespawnComponent.generated.h"

class AFPSCharacter;
class UClass;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API URespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URespawnComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void StartRespawnTimer();
	void SetPawnToSpawn(TSubclassOf<AFPSCharacter> ClassToSpawn);

protected:
	virtual void BeginPlay() override;

private:
	void RespawnPlayer();
	TSubclassOf<AFPSCharacter> PawnToSpawn;
};
