// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupRespawnPoint.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/ChildActorComponent.h"
#include "Pickup.h"
#include "FPSCharacter.h"
#include "Weapon.h"

APickupRespawnPoint::APickupRespawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	RespawnPointCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RespawnPointCollision"));
	RootComponent = RespawnPointCollision;
	RespawnPointCollision->SetCollisionResponseToChannel(PROJECTILE_OBJ, ECR_Ignore);
	RespawnPointCollision->SetCollisionResponseToChannel(PICKUP_TRACE, ECR_Ignore);

	PickupComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("PickupComponent"));
	PickupComponent->SetupAttachment(RootComponent);
	PickupComponent->SetAutoActivate(true);

	RespawnDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("RespawnDecal"));
	RespawnDecal->SetupAttachment(RootComponent);
	RespawnDecal->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));
	RespawnDecal->SetWorldLocation(FVector(0.0f, 0.0f, -RespawnPointCollision->GetScaledSphereRadius()));
	RespawnDecal->DecalSize = FVector(8.0f, 32.0f, 32.0f);

	PickupVisualEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PickupVisualEffect"));
	PickupVisualEffect->SetupAttachment(RootComponent);
	PickupVisualEffect->SetAutoActivate(false);

	PickupSoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("PickupSoundEffect"));
	PickupSoundEffect->SetupAttachment(RootComponent);

	OccupyingCharacter = nullptr;
	PickupRespawnTime = 15.0f;
}

void APickupRespawnPoint::BeginPlay()
{
	Super::BeginPlay();	

	ChildPickupRef = Cast<APickup>(PickupComponent->GetChildActor());
	Cast<AWeapon>(ChildPickupRef)->bIsPickup = true;

 	RespawnPointCollision->OnComponentBeginOverlap.AddDynamic(this, &APickupRespawnPoint::BeginOverlap);
 	RespawnPointCollision->OnComponentEndOverlap.AddDynamic(this, &APickupRespawnPoint::EndOverlap);
}

void APickupRespawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OccupyingCharacter && PickupComponent->IsActive())
	{
		if (ChildPickupRef->TryApplyToCharacter(OccupyingCharacter))
		{
			DeactivatePickup();
			StartPickupActivationTimer();
		}
	}
}

void APickupRespawnPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	PickupComponent->SetChildActorClass(PickupType);
}

void APickupRespawnPoint::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	OccupyingCharacter = Cast<AFPSCharacter>(OtherActor);
}

void APickupRespawnPoint::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == OccupyingCharacter)
		OccupyingCharacter = nullptr;
}

void APickupRespawnPoint::DeactivatePickup()
{
	ChildPickupRef->HideMesh();
	PickupComponent->Deactivate();
	if (PickupVisualEffect)
		PickupVisualEffect->ActivateSystem(true);
	if (PickupSoundEffect)
		PickupSoundEffect->Activate(true);
}

void APickupRespawnPoint::StartPickupActivationTimer()
{
	GetWorldTimerManager().SetTimer(PickupActivationTimer, this, &APickupRespawnPoint::ActivatePickup, PickupRespawnTime, false);
}

void APickupRespawnPoint::ActivatePickup()
{
	PickupComponent->Activate();
	ChildPickupRef->ShowMesh();
}
