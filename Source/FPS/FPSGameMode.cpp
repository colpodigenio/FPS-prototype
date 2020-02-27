// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGameMode.h"
#include "Pickup.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameInstance.h"
#include "Components/ControllerComponentsContainer.h"
#include "Components/ScoreHandlingComponent.h"
#include "FPSPlayerController.h"
#include "RespawnPoint.h"
#include "FPSCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"

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
	AIControllerClass = AAIController::StaticClass();
}

void AFPSGameMode::BeginPlay()
{
	Super::BeginPlay();
	UFPSGameInstance* GI = Cast<UFPSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI->IsValidLowLevel())
	{
		BotDifficulty = GI->GetDifficulty();
		NumberOfPlayers = GI->GetNumberOfPlayers();
	}
	SpawnPlayers();
}

void AFPSGameMode::SpawnPlayers()
{
	TArray<AActor*> RespawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARespawnPoint::StaticClass(), RespawnPoints);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	int32 RandInt = UKismetMathLibrary::RandomIntegerInRange(0, RespawnPoints.Num() - 1);
	APawn* PlayerPawn = GetWorld()->SpawnActor<AFPSCharacter>(DefaultPawnClass, RespawnPoints[RandInt]->GetActorTransform(), SpawnParams);
	RespawnPoints.RemoveAt(RandInt);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(PlayerPawn);
	NumberOfPlayers--;
	while (NumberOfPlayers > 0)
	{
		NumberOfPlayers--;
		APawn* Pawn = GetWorld()->SpawnActor<AFPSCharacter>(DefaultPawnClass, RespawnPoints[NumberOfPlayers]->GetActorTransform(), SpawnParams);
		GetWorld()->SpawnActor<AAIController>(AIControllerClass, RespawnPoints[NumberOfPlayers]->GetActorTransform(), SpawnParams)->Possess(Pawn);
	}
}

