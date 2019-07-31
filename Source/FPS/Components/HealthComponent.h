// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	int32 HealthMax;
	int32 HealthMaxBoosted;
	int32 Health;
	int32 ArmorMax;
	int32 Armor;
	bool bIsDead;
	bool bIsRegenerating;
	int32 RegenerationDuration; // in seconds

	FTimerHandle RegenerateHealthTimer;
	FTimerHandle RevertHealthToMaxTimer;

	void RegenerateHealth(); 
	void RevertHealthToMax();
	void Die();

public:	
	UHealthComponent();

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetHealth() const { return Health; };
	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetArmor() const { return Armor; };
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsDead() const { return bIsDead; };
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsRegenerating() const { return bIsRegenerating; };

	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void ApplyHeal(int32 HealDelta);
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void AddArmor(int32 ArmorDelta);
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void ApplyDamage(int32 DamageDelta);
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void StartRegenerateHealthTimer(); // this function is called by regeneration power up
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void StartRevertHealthToMaxTimer();
};
