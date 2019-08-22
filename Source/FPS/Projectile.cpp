// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon.h"

// Maybe it is good to use AbstractFactory to implement projectiles?

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	ProjectileMovement = CreateAbstractDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	DamageDone = 20.0f;

	ProjectileMovement->bInitialVelocityInLocalSpace = false;
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

void AProjectile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetProjectileInitialVelocity();
}

void AProjectile::SetProjectileInitialVelocity()
{
	ProjectileMovement->Velocity = FindShotDirection();
}

FVector AProjectile::FindShotDirection()
{
	AWeapon* OwningWeapon = Cast<AWeapon>(GetOwner());
	if (!OwningWeapon)
		return FVector(0.0f);
	ensureMsgf(OwningWeapon, TEXT("When shooting this projectile you should set the shooting weapon as an owner in FActorSpawnParameters"));
	FHitResult HitResult;
	AFPSCharacter* OwningCharacter = Cast<AFPSCharacter>(OwningWeapon->GetOwner());
	GetWorld()->LineTraceSingleByChannel(HitResult, OwningCharacter->GetFPSCamera()->GetComponentLocation(),
		OwningCharacter->GetFPSCamera()->GetComponentLocation() + 100000 * OwningCharacter->GetFPSCamera()->GetForwardVector(), ECollisionChannel::ECC_Visibility);
	FVector DirectionStartPoint = OwningWeapon->GetWeaponMesh()->GetSocketLocation("Muzzle");
	FVector DirectionEndPoint;
	if(HitResult.bBlockingHit)
		DirectionEndPoint = HitResult.ImpactPoint;
	else
		DirectionEndPoint = HitResult.TraceEnd;
	FVector ShotDirection = (DirectionEndPoint - DirectionStartPoint).GetSafeNormal();
	return ShotDirection;
}

#include "DrawDebugHelpers.h"

void AProjectile::HitTarget(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor->GetClass() != this->GetClass())
		Destroy();
	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 8.0f, 16, FColor::Emerald, true);
	if (Cast<AFPSCharacter>(OtherActor))
	{
// 		AFPSCharacter* Enemy = Cast<AFPSCharacter>(OtherActor);
// 		if(Hit.PhysMaterial->SurfaceType == "Head");
// 			Enemy->ReceiveDamage(3 * DamageDone);
// 		else
// 			Enemy->ReceiveDamage(DamageDone);
	}
}

