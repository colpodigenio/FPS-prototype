// Fill out your copyright notice in the Description page of Project Settings.

#include "ScoreHandlingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameMode.h"
#include "FPS.h"
#include "DeathmatchGameMode.h"

UScoreHandlingComponent::UScoreHandlingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	GM = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void UScoreHandlingComponent::BeginPlay()
{
	Super::BeginPlay();

// 	if(GetOwner()->IsValidLowLevel())
// 	PlayerName = *GetOwner()->GetName();
	
	SetPlayerNameInGM();	
	Cast<ADeathmatchGameMode>(GM)->ScoreBoard.Add(PlayerName, Score);
}

void UScoreHandlingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SendScoreToGM();
	PRINT("%s", *GetOwner()->GetName())
}

void UScoreHandlingComponent::SetPlayerNameInGM()
{
	GM->PlayersNames.Add(PlayerName);
}

void UScoreHandlingComponent::AddFrag()
{
	Score.Frags++;
}

void UScoreHandlingComponent::AddDeath()
{
	Score.Deaths++;
}

void UScoreHandlingComponent::AddSuicide()
{
	Score.Suicides++;
}

void UScoreHandlingComponent::SendScoreToGM()
{
	Cast<ADeathmatchGameMode>(GM)->ScoreBoard.Add(PlayerName, Score);
}


