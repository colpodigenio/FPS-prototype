// Fill out your copyright notice in the Description page of Project Settings.

#include "ScoreHandlingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ControllerComponentsContainer.h"
#include "FPSGameMode.h"
#include "FPS.h"
#include "DeathmatchGameMode.h"
#include "FPSPlayerController.h"
#include "FPSGameInstance.h"
#include "FPSAIController.h"

UScoreHandlingComponent::UScoreHandlingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	GM = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void UScoreHandlingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SendScoreToGM();
	if (Cast<AFPSPlayerController>(GetOwner()))
		PlayerName = Cast<UFPSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->ChosenPlayerName;
	else
	{
		Cast<AFPSAIController>(GetOwner())->BotCount++;
		PlayerName = TEXT("Bot");
		PlayerName.Append(FString::FromInt(Cast<AFPSAIController>(GetOwner())->BotCount));
	}
	SetPlayerNameInGM();
}

void UScoreHandlingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SendScoreToGM();
}

void UScoreHandlingComponent::SetPlayerNameInGM()
{
	if (GM->IsValidLowLevel())
	{
		GM->PlayersNames.Add(PlayerName);
		if (Cast<AFPSPlayerController>(GetOwner()))
			GM->HumanPlayerName = PlayerName;
	}
}

void UScoreHandlingComponent::AddFrag()
{
	PlayerData.DMScore.Frags++;
	CalculateScore(20);
}

void UScoreHandlingComponent::AddDeath()
{
	PlayerData.DMScore.Deaths++;
	CalculateScore(-5);
}

void UScoreHandlingComponent::AddSuicide()
{
	PlayerData.DMScore.Suicides++;
	CalculateScore(-5);
}

void UScoreHandlingComponent::CalculateScore(int32 ScoreDelta)
{
	PlayerData.DMScore.Score += ScoreDelta;
	if (PlayerData.DMScore.Score >= Cast<ADeathmatchGameMode>(GM)->GetPointsToWin())
		Cast<ADeathmatchGameMode>(GM)->EndMatch(PlayerName);
}

void UScoreHandlingComponent::SetPlayerName(FString NewName)
{
	PlayerName = NewName;
}

void UScoreHandlingComponent::SendScoreToGM()
{
	if (Cast<ADeathmatchGameMode>(GM)->IsValidLowLevel())
		Cast<ADeathmatchGameMode>(GM)->ScoreBoard.Add(PlayerName, PlayerData.DMScore);
}


