// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSPlayerController.h"
#include "FPSGameMode.h"
#include "FPS.h"
#include "Components/ScoreHandlingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpectatorPawn.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/KismetMathLibrary.h"
#include "FPSCharacter.h"

AFPSPlayerController::AFPSPlayerController()
{
	ScoreHandlingComponent = CreateDefaultSubobject<UScoreHandlingComponent>(TEXT("ScoreHandlingComponent"));
}

void AFPSPlayerController::SpawnAndPossesSpectator()
{
	ASpectatorPawn* Spectator = GetWorld()->SpawnActor<ASpectatorPawn>(ASpectatorPawn::StaticClass(), GetPawn()->GetActorTransform());
	Possess(Spectator);
}

void AFPSPlayerController::StartRespawnTimer()
{
	FTimerHandle RespawnTimer;
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &AFPSPlayerController::RespawnPlayer, 5.0f, false);
}

void AFPSPlayerController::RespawnPlayer()
{
	// Should choose furthest from enemies not random??
	TArray<AActor*> PlayerStartPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStartPoints);
	int PlayerStartIndex = UKismetMathLibrary::RandomIntegerInRange(0, PlayerStartPoints.Num() - 1);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AFPSCharacter* NewPawn = GetWorld()->SpawnActor<AFPSCharacter>(PawnToSpawn, PlayerStartPoints[PlayerStartIndex]->GetActorTransform(), SpawnParams);
	Possess(NewPawn);
}
