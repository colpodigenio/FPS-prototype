// Fill out your copyright notice in the Description page of Project Settings.

#include "ControllerComponentsContainer.h"
#include "RespawnComponent.h"
#include "ScoreHandlingComponent.h"

UControllerComponentsContainer::UControllerComponentsContainer()
{
	PrimaryComponentTick.bCanEverTick = true;

	RespawnComp = CreateDefaultSubobject<URespawnComponent>(TEXT("RespawnComponent"));
	ScoreHandlingComp = CreateDefaultSubobject<UScoreHandlingComponent>(TEXT("ScoreHandling"));
}

void UControllerComponentsContainer::BeginPlay()
{
	Super::BeginPlay();

	//ScoreHandlingComp->SetPlayerName(PlayerName);
	RespawnComp->SetPawnToSpawn(PawnToSpawn);
}

void UControllerComponentsContainer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

