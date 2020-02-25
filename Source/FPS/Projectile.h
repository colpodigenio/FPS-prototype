// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPS.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;
class AFPSCharacter;

UCLASS()
class FPS_API AProjectile : public AActor
{
	GENERATED_BODY()

public:	

	AProjectile();
	virtual void OnConstruction(const FTransform& Transform) override;
	UPROPERTY(BlueprintReadOnly)
	AController* DamageInstigator;
	UFUNCTION(BlueprintImplementableEvent)
	void PlayImpactEffect(UPhysicalMaterial* ImpactPhysMat);

protected:

	UPROPERTY(EditDefaultsOnly)
	int32 DamageDone;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	virtual void BeginPlay() override;
	virtual void SetProjectileInitialVelocity();
	UFUNCTION()
	virtual void HitTarget(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void ReportDamageSense(AFPSCharacter* EnemyToReport);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
};
