// Fill out your copyright notice in the Description page of Project Settings.

#include "ShotgunPellet.h"
#include "FPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon.h"




FVector AShotgunPellet::FindShotDirection()
{
	AWeapon* OwningWeapon = Cast<AWeapon>(GetOwner());
	if (!OwningWeapon)
		return FVector(0.0f);
	ensureMsgf(OwningWeapon, TEXT("When shooting this projectile you should set the shooting weapon as an owner in FActorSpawnParameters"));
	FHitResult HitResult;
	AFPSCharacter* OwningCharacter = Cast<AFPSCharacter>(OwningWeapon->GetOwner());
	GetWorld()->LineTraceSingleByChannel(HitResult, OwningCharacter->GetFPSCamera()->GetComponentLocation(),
		OwningCharacter->GetFPSCamera()->GetComponentLocation() + 100000 * UKismetMathLibrary::RandomUnitVectorInConeInDegrees(OwningCharacter->GetFPSCamera()->GetForwardVector(), 10.f), ECollisionChannel::ECC_Visibility);
	FVector DirectionStartPoint = OwningWeapon->GetWeaponMesh()->GetSocketLocation("Muzzle");
	FVector DirectionEndPoint;
	if (HitResult.bBlockingHit)
		DirectionEndPoint = HitResult.ImpactPoint;
	else
		DirectionEndPoint = HitResult.TraceEnd;
	FVector ShotDirection = (DirectionEndPoint - DirectionStartPoint).GetSafeNormal();
	return ShotDirection;
}
