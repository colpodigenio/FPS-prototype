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
	if(Mesh)
		Mesh->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool APickup::TryApplyToCharacter(AFPSCharacter* Character)
{
	return true;
}

