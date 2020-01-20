// Fill out your copyright notice in the Description page of Project Settings.

#include "DeathmatchGameMode.h"
#include "DeathmatchPlayerController.h"

ADeathmatchGameMode::ADeathmatchGameMode()
{
	PlayerControllerClass = ADeathmatchPlayerController::StaticClass();
}
