// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIController.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameMode.h"
#include "Kismet/KismetMathLibrary.h"

void AFPSAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn /*= true*/)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		FRotator NewControlRotation = GetControlRotation();

		// Look toward focus
		const FVector FocalPoint = GetFocalPoint();
		if (FAISystem::IsValidLocation(FocalPoint))
		{
			NewControlRotation = (FocalPoint - MyPawn->GetPawnViewLocation()).Rotation() - AimingDeviation;
		}
		else if (bSetControlRotationFromPawnOrientation)
		{
			NewControlRotation = MyPawn->GetActorRotation();
		}

		// Don't pitch view unless looking at another pawn
		if (NewControlRotation.Pitch != 0 && Cast<APawn>(GetFocusActor()) == nullptr)
		{
			NewControlRotation.Pitch = 0.f;
		}

		SetControlRotation(NewControlRotation);

		if (bUpdatePawn)
		{
			const FRotator CurrentPawnRotation = MyPawn->GetActorRotation();

			if (CurrentPawnRotation.Equals(NewControlRotation, 1e-3f) == false)
			{
				MyPawn->FaceRotation(NewControlRotation, DeltaTime);
			}
		}
	}
}

void AFPSAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetRandomAimingDeviation();
}

void AFPSAIController::BeginPlay()
{
	Super::BeginPlay();

	SetRandomAimingDeviation();
}

void AFPSAIController::SetRandomAimingDeviation()
{
	EBotDifficulty::Type BotDifficulty = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->BotDifficulty;
	float TargetsSpeedDeviationRatio = 0.0f;
	if (GetFocusActor())
		TargetsSpeedDeviationRatio = GetFocusActor()->GetVelocity().Size() / EMovementState::Sprinting;
		
	float RandomDeviation = BotDifficulty * UKismetMathLibrary::RandomFloatInRange(0.0f, 2.5f) * TargetsSpeedDeviationRatio;

	AimingDeviation = FRotator(RandomDeviation, RandomDeviation, 0.0f);
}
