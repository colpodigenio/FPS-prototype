// Fill out your copyright notice in the Description page of Project Settings.

#include "ShotgunPellet.h"
#include "FPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon.h"

AShotgunPellet::AShotgunPellet()
	: Super(){}

void AShotgunPellet::SetProjectileInitialVelocity()
{
	AWeapon* OwningWeapon = Cast<AWeapon>(GetOwner());
	if (!OwningWeapon)
		return;

	ensureMsgf(OwningWeapon, TEXT("When shooting this projectile you should set the shooting weapon as an owner in FActorSpawnParameters"));
	FHitResult HitResult;
	AFPSCharacter* OwningCharacter = Cast<AFPSCharacter>(OwningWeapon->GetOwner());
	GetWorld()->LineTraceSingleByChannel(HitResult, OwningCharacter->GetFPSCameraLocation(),
		OwningCharacter->GetFPSCameraLocation() + 100000 * UKismetMathLibrary::RandomUnitVectorInConeInDegrees(OwningCharacter->GetFPSCameraForwardVector(), 10.f), ENEMY_TRACE);
	FVector DirectionStartPoint = OwningWeapon->GetWeaponMesh()->GetSocketLocation("Muzzle");
	FVector DirectionEndPoint;
	if (HitResult.bBlockingHit)
		DirectionEndPoint = HitResult.ImpactPoint;
	else
		DirectionEndPoint = HitResult.TraceEnd;
	FVector ShotDirection = (DirectionEndPoint - DirectionStartPoint).GetSafeNormal();
	ProjectileMovement->Velocity = ShotDirection;
}

