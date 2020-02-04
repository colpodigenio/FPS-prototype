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
	
	SetPlayerNameInGM();
	SendScoreToGM();
}

void UScoreHandlingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SendScoreToGM();
}

void UScoreHandlingComponent::SetPlayerNameInGM()
{
	if(GM->IsValidLowLevel())
		GM->PlayersNames.Add(PlayerName);
}

void UScoreHandlingComponent::AddFrag()
{
	Score.Frags++;
	CalculateScore(20);
}

void UScoreHandlingComponent::AddDeath()
{
	Score.Deaths++;
	CalculateScore(-5);
}

void UScoreHandlingComponent::AddSuicide()
{
	Score.Suicides++;
	CalculateScore(-5);
}

void UScoreHandlingComponent::CalculateScore(int32 ScoreDelta)
{
	Score.Score += ScoreDelta;
}

void UScoreHandlingComponent::SetPlayerName(FName NewName)
{
	PlayerName = NewName;
}

void UScoreHandlingComponent::SendScoreToGM()
{
	if (Cast<ADeathmatchGameMode>(GM)->IsValidLowLevel())
		Cast<ADeathmatchGameMode>(GM)->ScoreBoard.Add(PlayerName, Score);
}


