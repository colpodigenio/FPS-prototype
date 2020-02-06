// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalizationLibrary.h"

void ULocalizationLibrary::ChangeLanguage(const FString &Target)
{
	FInternationalization::Get().SetCurrentCulture(Target);
}
