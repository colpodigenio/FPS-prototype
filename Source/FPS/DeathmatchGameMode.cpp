// Fill out your copyright notice in the Description page of Project Settings.

#include "DeathmatchGameMode.h"
#include "DeathmatchPlayerController.h"
#include "Components/ControllerComponentsContainer.h"
#include "Components/ScoreHandlingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameInstance.h"

ADeathmatchGameMode::ADeathmatchGameMode()
{
	PlayerControllerClass = ADeathmatchPlayerController::StaticClass();
	PointsToWin = 20;
}

void ADeathmatchGameMode::EndMatch(FString WinnerName)
{
	UFPSGameInstance* GameInstance = Cast<UFPSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	AFPSPlayerController* PC = Cast<AFPSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	FPlayerProfileData NewPlayerProfileData = GameInstance->PlayersProfileData.FindRef(WinnerName);
	bHasMatchPlayed = true;
	if (GameInstance->PlayersProfileData.Contains(WinnerName))
		NewPlayerProfileData.DeathmatchesWon++;
	else
		NewPlayerProfileData.DeathmatchesLost++;
	GameInstance->PlayersProfileData.Add(PC->GetControllerComponentsContainer()->GetScoreHandlingComponent()->GetPlayerName()) = NewPlayerProfileData;
	PC->ShowScoreboard();
	FTimerHandle MenuTimer;
	GetWorldTimerManager().SetTimer(MenuTimer, this, &ADeathmatchGameMode::ReturnToLobby, 5.0f, false);
}

void ADeathmatchGameMode::ReturnToLobby()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Menu"));
}

