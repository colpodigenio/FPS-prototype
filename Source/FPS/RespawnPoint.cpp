// Fill out your copyright notice in the Description page of Project Settings.

#include "RespawnPoint.h"
#include "Components/CapsuleComponent.h"


// Sets default values
ARespawnPoint::ARespawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;
	CapsuleComponent->SetGenerateOverlapEvents(true);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	CapsuleComponent->SetCapsuleHalfHeight(89.0f);
	CapsuleComponent->SetCapsuleRadius(34.f);
}


