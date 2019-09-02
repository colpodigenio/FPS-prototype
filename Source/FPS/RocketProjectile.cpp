// Fill out your copyright notice in the Description page of Project Settings.

#include "RocketProjectile.h"
#include "FPSCharacter.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "WorldCollision.h"
#include "Components/CapsuleComponent.h"

#include "DrawDebugHelpers.h"

void ARocketProjectile::HitTarget(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	OtherComp->AddImpulseAtLocation(GetVelocity() * 10.0f, GetActorLocation());

	FHitResult SphereHit;
	GetWorld()->SweepSingleByChannel(SphereHit, Hit.ImpactPoint, Hit.ImpactPoint, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(200.f));
	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 200.f, 12, FColor::Black, false, 5.f, 0, 1.0f);
	AFPSCharacter* Enemy = Cast<AFPSCharacter>(SphereHit.GetActor());
	if (Enemy)
	{
		//OtherComp->AddRadialImpulse(SphereHit.ImpactPoint - Hit.ImpactPoint, 200.f, 1000.f, ERadialImpulseFalloff::RIF_Linear);
		float DistanceToEnemy = (SphereHit.ImpactPoint - Hit.ImpactPoint).Size();
		UE_LOG(LogTemp, Warning, TEXT("%s receives damage = %f, distance to enemy = %f"), *SphereHit.GetActor()->GetName(), DamageDone * DistanceToEnemy / 200.0f, DistanceToEnemy)
 		Enemy->ReceiveDamage(DamageDone * DistanceToEnemy / 200.0f);
		ReportDamageSense(Enemy);
	}
	Destroy();
}
