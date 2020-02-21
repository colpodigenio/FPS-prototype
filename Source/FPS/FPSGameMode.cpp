// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGameMode.h"
#include "Pickup.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameInstance.h"
#include "Components/ControllerComponentsContainer.h"
#include "Components/ScoreHandlingComponent.h"
#include "FPSPlayerController.h"

AFPSGameMode::AFPSGameMode()
{
	int32 id = 0;
	TArray<AActor*> AllPickups;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickup::StaticClass(), AllPickups);
	for (auto& It : AllPickups)
	{
		if (!It) return;
		Cast<APickup>(It)->PickupID = id;
		id++;
	}

	BotDifficulty = EBotDifficulty::Easy;
}

