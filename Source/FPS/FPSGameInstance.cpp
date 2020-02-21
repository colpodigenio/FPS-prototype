// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "FPSProfileSave.h"

void UFPSGameInstance::Init()
{
	Super::Init();
	UFPSProfileSave* ProfileSave = Cast<UFPSProfileSave>(UGameplayStatics::LoadGameFromSlot(TEXT("Profiles"), 0));
	if (ProfileSave->IsValidLowLevel())
	{
		PlayersProfileData = ProfileSave->PlayerProfileData;
		ChosenPlayerName = ProfileSave->LastChosenPlayerName;
	}
}
