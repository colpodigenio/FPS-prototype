// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSPlayerController.h"
#include "FPSGameMode.h"
#include "Components/ScoreHandlingComponent.h"
#include "Kismet/GameplayStatics.h"

AFPSPlayerController::AFPSPlayerController()
{
	ScoreHandlingComponent = CreateDefaultSubobject<UScoreHandlingComponent>(TEXT("ScoreHandlingComponent"));
}

void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();
}
