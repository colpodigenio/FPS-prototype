// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "FPSProfileSave.h"
#include "FPSConfigSave.h"

void UFPSGameInstance::Init()
{
	Super::Init();
	UFPSProfileSave* ProfileSave = Cast<UFPSProfileSave>(UGameplayStatics::LoadGameFromSlot(TEXT("Profiles"), 0));
	if (ProfileSave->IsValidLowLevel())
	{
		PlayersProfileData = ProfileSave->PlayersProfileData;
		ChosenPlayerName = ProfileSave->LastChosenPlayerName;
	}
	UFPSConfigSave* ConfigSave = Cast<UFPSConfigSave>(UGameplayStatics::LoadGameFromSlot(TEXT("Config"), 0));
	if (ConfigSave->IsValidLowLevel())
	{
		MouseSensitivity = ConfigSave->MouseSensitivity;
	}
	BotDifficulty = EBotDifficulty::Easy;
	NumberOfPlayers = 2;
}
