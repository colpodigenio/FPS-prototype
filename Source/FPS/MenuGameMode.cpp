// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuGameMode.h"
#include "FPSPlayerController.h"

AMenuGameMode::AMenuGameMode()
{
	PlayerControllerClass = AFPSPlayerController::StaticClass();
}
