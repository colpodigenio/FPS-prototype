// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBlueprintFunctionLibrary.h"
#include "FPS.h"
#include "ConfigCacheIni.h"

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