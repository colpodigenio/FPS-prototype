// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"
#include "FPSCharacter.h"
#include "Components/StaticMeshComponent.h"


FName APickup::MeshComponentName(TEXT("Mesh"));

APickup::APickup(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>(MeshComponentName);
	RootComponent = Mesh;
	if (Mesh)
	{
		Mesh->SetCollisionResponseToAllChannels(ECR_Overlap);
		//Mesh->SetCollisionResponseToChannel(Pickup, ECR_Block);
	}
	bIsPickupActive = true;
	PickupType = EPickupType::None;
	PickupValue = 1.0f;
}

void APickup::BeginPlay()
{
	Super::BeginPlay();
	ensureMsgf(PickupType > EPickupType::None, TEXT("Choose pickup type for %s"), *GetName());
}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool APickup::TryApplyToCharacter(AFPSCharacter* Character)
{
	return true;
}

void APickup::ShowMesh()
{
	if(Mesh)
		Mesh->SetVisibility(true);
	bIsPickupActive = true;
}

void APickup::HideMesh()
{
	if(Mesh)
		Mesh->SetVisibility(false);
	bIsPickupActive = false;
}

