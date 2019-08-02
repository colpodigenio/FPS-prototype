// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupRespawnPoint.h"

APickupRespawnPoint::APickupRespawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APickupRespawnPoint::BeginPlay()
{
	Super::BeginPlay();	
}

void APickupRespawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

