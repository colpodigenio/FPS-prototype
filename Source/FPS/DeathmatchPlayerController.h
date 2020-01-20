// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSPlayerController.h"
#include "FPS.h"
#include "DeathmatchPlayerController.generated.h"

UCLASS()
class FPS_API ADeathmatchPlayerController : public AFPSPlayerController
{
	GENERATED_BODY()
	
public:
	
	ADeathmatchPlayerController();

	FDeathmatchScore PlayerScore;
};
