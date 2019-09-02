// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Perception/AISense_Damage.h"
#include "Kismet/KismetMathLibrary.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionObjectType(Projectile);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(Projectile, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(EnemyTrace, ECR_Block);
	Mesh->SetCollisionResponseToChannel(EnemyObj, ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	ProjectileMovement = CreateAbstractDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	DamageDone = 20.0f;

	ProjectileMovement->bInitialVelocityInLocalSpace = false;
}


void AProjectile::BeginPlay()
{
	Super::BeginPlay();	
	Mesh->OnComponentHit.AddDynamic(this, &AProjectile::HitTarget);
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

#include "DrawDebugHelpers.h"
FVector AProjectile::FindShotDirection()
{
	AWeapon* OwningWeapon = Cast<AWeapon>(GetOwner());
	if (!OwningWeapon)
		return FVector(0.0f);

	FHitResult HitResult;
	AFPSCharacter* OwningCharacter = Cast<AFPSCharacter>(OwningWeapon->GetOwner());
	GetWorld()->LineTraceSingleByChannel(HitResult, OwningCharacter->GetFPSCamera()->GetComponentLocation(),
		OwningCharacter->GetFPSCamera()->GetComponentLocation() + 100000 * OwningCharacter->GetFPSCamera()->GetForwardVector(), EnemyTrace);
	FVector DirectionStartPoint = OwningWeapon->GetWeaponMesh()->GetSocketLocation("Muzzle");
	FVector DirectionEndPoint;
	HitResult.bBlockingHit ? DirectionEndPoint = HitResult.ImpactPoint : DirectionEndPoint = HitResult.TraceEnd;
	FVector ShotDirection = (DirectionEndPoint - DirectionStartPoint).GetSafeNormal();
	return ShotDirection;
}


void AProjectile::HitTarget(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 8.0f, 16, FColor::Emerald, true);
	AFPSCharacter* Enemy = Cast<AFPSCharacter>(OtherActor);
	if (Enemy)
	{
		if (Hit.PhysMaterial == nullptr)
			return;
		if (Hit.PhysMaterial->SurfaceType.GetValue() == Head)
			Enemy->ReceiveDamage(3 * DamageDone);
		else
			Enemy->ReceiveDamage(DamageDone);
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


