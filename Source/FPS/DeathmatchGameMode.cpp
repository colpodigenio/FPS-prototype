// Fill out your copyright notice in the Description page of Project Settings.

#include "DeathmatchGameMode.h"
#include "DeathmatchPlayerController.h"
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
	PC->ShowScoreboard();
	FTimerHandle MenuTimer;
	GetWorldTimerManager().SetTimer(MenuTimer, this, &ADeathmatchGameMode::ReturnToLobby, 5.0f, false);
}

void ADeathmatchGameMode::ReturnToLobby()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Menu"));
}

