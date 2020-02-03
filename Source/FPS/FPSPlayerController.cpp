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

AFPSPlayerController::AFPSPlayerController()
{
	ContrCompContainer = CreateDefaultSubobject<UControllerComponentsContainer>(TEXT("ControllerComponentsContainer"));
}

void AFPSPlayerController::SpawnAndPossesSpectator()
{
	ASpectatorPawn* Spectator = GetWorld()->SpawnActor<ASpectatorPawn>(ASpectatorPawn::StaticClass(), GetPawn()->GetActorTransform());
	Possess(Spectator);
}


