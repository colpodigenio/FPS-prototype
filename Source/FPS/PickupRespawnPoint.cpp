// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupRespawnPoint.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/ChildActorComponent.h"
#include "Pickup.h"
#include "FPSCharacter.h"

APickupRespawnPoint::APickupRespawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	RespawnPointCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RespawnPointCollision"));
	RootComponent = RespawnPointCollision;

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

	PickupSoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("PickupSoundEffect"));
	PickupSoundEffect->SetupAttachment(RootComponent);

	bIsPickupPointOccupied = false;
	OccupyingCharacter = nullptr;
	PickupRespawnTime = 15.0f;
}

void APickupRespawnPoint::BeginPlay()
{
	Super::BeginPlay();	

 	RespawnPointCollision->OnComponentBeginOverlap.AddDynamic(this, &APickupRespawnPoint::BeginOverlap);
 	RespawnPointCollision->OnComponentEndOverlap.AddDynamic(this, &APickupRespawnPoint::EndOverlap);
}

void APickupRespawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPickupPointOccupied && PickupComponent->IsActive())
	{
		ApplyPickupToCharacter();
		DeactivatePickup();
		StartPickupActivationTimer();
	}
}

void APickupRespawnPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	PickupComponent->SetChildActorClass(PickupType);
}

void APickupRespawnPoint::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("BeginOverlap"))
	OccupyingCharacter = Cast<AFPSCharacter>(OtherActor);
	if (OccupyingCharacter)
	{
		bIsPickupPointOccupied = true;
		UE_LOG(LogTemp, Warning, TEXT("OccupyingCharacter"))
	}
}

void APickupRespawnPoint::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("EndOverlap"))
	if (OtherActor == OccupyingCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("nullptr"))
		bIsPickupPointOccupied = false;
		OccupyingCharacter = nullptr;
	}
}

void APickupRespawnPoint::DeactivatePickup()
{
	UE_LOG(LogTemp, Warning, TEXT("Deactivate"))
	PickupComponent->Deactivate();
}

void APickupRespawnPoint::ApplyPickupToCharacter()
{
	APickup* PickupInstance = Cast<APickup>(PickupComponent->GetChildActor());
	PickupInstance->AddThisToCharacterInventory(OccupyingCharacter);
}

void APickupRespawnPoint::StartPickupActivationTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("StartTimer"))
	GetWorldTimerManager().SetTimer(PickupActivationTimer, this, &APickupRespawnPoint::ActivatePickup, PickupRespawnTime, false);
}

void APickupRespawnPoint::ActivatePickup()
{
	UE_LOG(LogTemp, Warning, TEXT("Activate"))
	PickupComponent->Activate();
}