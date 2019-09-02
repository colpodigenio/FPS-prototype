// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FPS.h"
#include "PowerupComponent.generated.h"

class AFPSCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API UPowerupComponent : public UActorComponent
{
	GENERATED_BODY()

	void EnableSpeedBoost();
	void EnableDamageBoost();
	void EnableRegeneration();
	void DisableSpeedBoost();
	void DisableDamageBoost();
	void DisableRegeneration();
	bool bIsPowerupEnabled;
	float SpeedBoostDuration;
	float DamageBoostDuration;
	int32 RegenerationDuration;
	TWeakObjectPtr<AFPSCharacter> OwningCharacter;
	FTimerHandle PowerupTimer;

protected:
	virtual void BeginPlay() override;

public:	
	UPowerupComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void EnablePowerup(EPowerupType PowerupType);
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsPowerupEnabled() const { return bIsPowerupEnabled; };
	
};
