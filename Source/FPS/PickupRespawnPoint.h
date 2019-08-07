// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupRespawnPoint.generated.h"

class USphereComponent;
class APickup;
class UDecalComponent;
class UParticleSystemComponent;
class UAudioComponent;
class AFPSCharacter;

UCLASS()
class FPS_API APickupRespawnPoint : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* RespawnPointCollision;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* PickupComponent;
	UPROPERTY(EditAnywhere)
	TSubclassOf<APickup> PickupType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDecalComponent* RespawnDecal;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* PickupVisualEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* PickupSoundEffect;

	bool bIsPickupPointOccupied;
	AFPSCharacter* OccupyingCharacter;
	FTimerHandle PickupActivationTimer;
	float PickupRespawnTime;

	void ActivatePickup();
	void StartPickupActivationTimer();
	void DeactivatePickup();
	void ApplyPickupToCharacter();


public:	
	APickupRespawnPoint();
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;
	
};
