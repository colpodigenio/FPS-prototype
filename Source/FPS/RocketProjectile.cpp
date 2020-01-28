// Fill out your copyright notice in the Description page of Project Settings.

#include "RocketProjectile.h"
#include "FPSCharacter.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "WorldCollision.h"
#include "FPS.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


void ARocketProjectile::HitTarget(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherComp->Mobility == EComponentMobility::Movable)
		OtherComp->AddImpulseAtLocation(GetVelocity() * 10.0f, GetActorLocation());
	PlayImpactEffect(Hit.ImpactPoint, nullptr);
	TArray<AActor*> AllParticipants;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFPSCharacter::StaticClass(), AllParticipants);
	for (auto It : AllParticipants)
	{
		AFPSCharacter* Participant = Cast<AFPSCharacter>(It);
		float DistanceToParticipant = (Participant->GetActorLocation() - Hit.ImpactPoint).Size();
		if (Participant == OtherActor)
		{
			Participant->ReceiveDamage(DamageDone, DamageInstigator);
			continue;
		}
		if (DistanceToParticipant <= 200.f)
			Participant->ReceiveDamage(DamageDone * (1. - DistanceToParticipant / 300.f), DamageInstigator);
		else if (DistanceToParticipant < 50.f)
 			Participant->ReceiveDamage(DamageDone, DamageInstigator);
	}
	Destroy();
}
