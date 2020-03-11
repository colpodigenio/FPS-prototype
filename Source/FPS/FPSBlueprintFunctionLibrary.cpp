// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBlueprintFunctionLibrary.h"
#include "FPS.h"
#include "ConfigCacheIni.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"

void UFPSBlueprintFunctionLibrary::LoadInputConfig(UObject* Obj, FString Filename /*= NULL*/)
{
// 	FConfigFile OverrideConfig;
// 	FConfigCacheIni::LoadLocalIniFile(OverrideConfig, *Obj->GetDefaultConfigFilename(), true);
// 	TArray<FString> OutKeys;
// 	OverrideConfig.GetBool("/Script/Engine.InputSettings", "")
// 	OverrideConfig.GetKeys(OutKeys);
// 	OverrideConfig.
// 	for (auto It : OutKeys)
// 	{
// 		PRINT("SDFASD %s", *It)
// 	}
	//GInputIni = *Obj->GetDefaultConfigFilename();
// 	PRINT("%s", *GInputIni)
	if(Filename.IsEmpty())
		Obj->LoadConfig();
	else
	{
		Obj->LoadConfig(Obj->GetClass(), *Obj->GetDefaultConfigFilename());
		PRINT("%s", *Obj->GetDefaultConfigFilename())
	}
// 	if (Filename.IsEmpty())
// 		Obj->ReloadConfig();
// 	else
// 	{
// 		Obj->ReloadConfig(NULL, *Obj->GetDefaultConfigFilename());
// 		PRINT("%s", *Obj->GetDefaultConfigFilename())
// 	}
}

float UFPSBlueprintFunctionLibrary::GetMouseSensitivity()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GEngine->GameViewport->GetWorld(), 0);
	return PC->PlayerInput->GetMouseSensitivityX();
}

void UFPSBlueprintFunctionLibrary::SetMouseSensitivity(float NewSensitivity)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GEngine->GameViewport->GetWorld(), 0);
	PC->PlayerInput->SetMouseSensitivity(NewSensitivity, NewSensitivity);
}
