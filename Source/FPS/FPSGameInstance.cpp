// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "FPSProfileSave.h"

void UFPSGameInstance::Init()
{
	Super::Init();
	if(UGameplayStatics::LoadGameFromSlot(TEXT("Profiles"), 0)->IsValidLowLevel())
		PlayerProfileData = Cast<UFPSProfileSave>(UGameplayStatics::LoadGameFromSlot(TEXT("Profiles"), 0))->PlayerProfileData;
}
