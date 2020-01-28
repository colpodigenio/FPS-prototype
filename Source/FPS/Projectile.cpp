// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon.h"
#include "FPS.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Perception/AISense_Damage.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Controller.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionObjectType(PROJECTILE_OBJ);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(PROJECTILE_OBJ, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ENEMY_TRACE, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ENEMY_OBJ, ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	DamageDone = 20.0f;

	ProjectileMovement->bInitialVelocityInLocalSpace = false;
}

#include "FPSPlayerController.h"
#include "Kismet/GameplayStatics.h"

void AProjectile::BeginPlay()
{
	Super::BeginPlay();	
	SetLifeSpan(2.0f);
	Mesh->OnComponentHit.AddDynamic(this, &AProjectile::HitTarget);
	DamageInstigator = Cast<AController>(Cast<APawn>(GetOwner()->GetOwner())->GetController()); // get controller which fires this projectile
}

void AProjectile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetProjectileInitialVelocity();
}

void AProjectile::SetProjectileInitialVelocity()
{
	AWeapon* OwningWeapon = Cast<AWeapon>(GetOwner());
	if (!OwningWeapon)
		return;
	AFPSCharacter* OwningCharacter = Cast<AFPSCharacter>(OwningWeapon->GetOwner());
	ProjectileMovement->Velocity = OwningCharacter->GetFPSCameraForwardVector();
}

#include "DrawDebugHelpers.h"

void AProjectile::HitTarget(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	PlayImpactEffect(Hit.ImpactPoint, Hit.PhysMaterial.Get());
	AFPSCharacter* Enemy = Cast<AFPSCharacter>(OtherActor);
	if (Enemy && Hit.PhysMaterial.Get())
	{
		if (UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get()) == HEAD_SURFACE)
 			Enemy->ReceiveDamage(3 * DamageDone, DamageInstigator);
		else
 			Enemy->ReceiveDamage(DamageDone, DamageInstigator);
		ReportDamageSense(Enemy);
	}
	Destroy();
}

void AProjectile::ReportDamageSense(AFPSCharacter* EnemyToReport)
{
	AFPSCharacter* WeaponOwner = Cast<AFPSCharacter>((GetOwner()->GetOwner()));
	FVector DamageInstigatorLocation = UKismetMathLibrary::RandomPointInBoundingBox(WeaponOwner->GetActorLocation(), FVector(400.0f, 400.0f, 50.0f));
	UAISense_Damage::ReportDamageEvent(GetWorld(), EnemyToReport, WeaponOwner, 0.0f, DamageInstigatorLocation, DamageInstigatorLocation);
}


