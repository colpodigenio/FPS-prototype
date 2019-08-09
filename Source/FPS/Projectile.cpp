// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FPSCharacter.h"

// Maybe it is good to use AbstractFactory to implement projectiles?

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	ProjectileMovement = CreateAbstractDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	DamageDone = 20.0f;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();	
	OnActorHit.AddDynamic(this, &AProjectile::HitTarget);
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::HitTarget(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
	if (Cast<AFPSCharacter>(OtherActor))
	{
// 		AFPSCharacter* Enemy = Cast<AFPSCharacter>(OtherActor);
// 		if(Hit.PhysMaterial->SurfaceType == "Head");
// 			Enemy->ReceiveDamage(3 * DamageDone);
// 		else
// 			Enemy->ReceiveDamage(DamageDone);
	}
}

