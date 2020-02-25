// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSPlayerController.h"
#include "FPSGameMode.h"
#include "FPS.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpectatorPawn.h"
#include "RespawnPoint.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "FPSCharacter.h"
#include "Components/ControllerComponentsContainer.h"
#include "Components/ScoreHandlingComponent.h"
#include "FPSGameInstance.h"
#include "FPSProfileSave.h"

AFPSPlayerController::AFPSPlayerController()
{
	ContrCompContainer = CreateDefaultSubobject<UControllerComponentsContainer>(TEXT("ControllerComponentsContainer"));
}

void AFPSPlayerController::SpawnAndPossesSpectator()
{
	ASpectatorPawn* Spectator = GetWorld()->SpawnActor<ASpectatorPawn>(ASpectatorPawn::StaticClass(), GetPawn()->GetActorTransform());
	Possess(Spectator);
}

void AFPSPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UFPSGameInstance* GameInstance = Cast<UFPSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	FString PlayerName = ContrCompContainer->GetScoreHandlingComponent()->GetPlayerName();
	FPlayerProfileData ProfileData = GameInstance->PlayersProfileData.FindRef(PlayerName);
		if (GameInstance->PlayersProfileData.Contains(PlayerName))
		{
			FPlayerProfileData NewProfileData;
			NewProfileData = GameInstance->PlayersProfileData.FindRef(PlayerName) + ContrCompContainer->GetScoreHandlingComponent()->GetPlayerData();
			GameInstance->PlayersProfileData.Add(PlayerName) = NewProfileData;
			UFPSProfileSave* ProfileSave = Cast<UFPSProfileSave>(UGameplayStatics::CreateSaveGameObject(UFPSProfileSave::StaticClass()));
			ProfileSave->PlayersProfileData = GameInstance->PlayersProfileData;
			ProfileSave->LastChosenPlayerName = GameInstance->ChosenPlayerName;
			UGameplayStatics::SaveGameToSlot(ProfileSave, TEXT("Profiles"), 0);
				// 		GameInstance->PlayersProfileData.FindRef(HumanPlayerName).DMScore.Score = GameInstance->PlayersProfileData.FindRef(HumanPlayerName).DMScore.Score
				// 			+ PC->GetControllerComponentsContainer()->GetScoreHandlingComponent()->GetPlayerData().DMScore.Score;
		}

}

