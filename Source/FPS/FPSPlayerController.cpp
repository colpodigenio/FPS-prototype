// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSPlayerController.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"

void AFPSPlayerController::SetPlayerNameInGM_Implementation()
{
	Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->PlayersNames.Add(*GetName());
	UE_LOG(LogTemp, Warning, TEXT("YES!!!!"))
}

void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (this->GetClass()->ImplementsInterface(UScoreCountingInterface::StaticClass()))
	{
		IScoreCountingInterface::Execute_SetPlayerNameInGM(this);
	};
}
