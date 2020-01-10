// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetHealth() const { return Health; };
	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetHealthMax() const { return HealthMax; };
	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetArmor() const { return Armor; };
	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetArmorMax() const { return ArmorMax; };
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsDead() const { return bIsDead; };
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsRegenerating() const { return bIsRegenerating; };

	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void ApplyHeal(int32 HealDelta);
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void AddArmor(int32 ArmorDelta, bool CanBoostMaxArmor);
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void ApplyDamage(int32 DamageDelta);
	UFUNCTION(BlueprintCallable, Category = "Health Component")
	void StartRegenerateHealthTimer(int32 NewRegenerationDuration); // this function is called by regeneration power up

private:

	int32 HealthMax;
	int32 HealthMaxBoosted;
	UPROPERTY(VisibleAnywhere)
	int32 Health;
	int32 ArmorMax;
	UPROPERTY(VisibleAnywhere)
	int32 Armor;
	bool bIsDead;
	bool bIsRegenerating;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	int32 HealthRegenerationDelta;
	int32 RegenerationDuration;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float RegenerationRate;

	FTimerHandle RegenerateHealthTimer;
	FTimerHandle RevertHealthToMaxTimer;

	void RegenerateHealth(); 
	void StartRevertHealthToMaxTimer();
	void RevertHealthToMax();
	void StopRevertHealthToMaxTimer();
	void Die();
};
