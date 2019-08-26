// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerupComponent.h"
#include "FPS.h"
#include "FPSCharacter.h"
#include "Components/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UPowerupComponent::UPowerupComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bIsPowerupEnabled = false;
	SpeedBoostDuration = 10.0f;
	DamageBoostDuration = 10.0f;
	RegenerationDuration = 10;
}

void UPowerupComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<AFPSCharacter>(GetOwner());
}

void UPowerupComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPowerupComponent::EnablePowerup(EPowerupType PowerupType)
{
	bIsPowerupEnabled = true;
	switch (PowerupType)
	{
	case EPowerupType::SpeedBoost:
		EnableSpeedBoost();
		return;
	case EPowerupType::DamageBoost:
		EnableDamageBoost();
		return;
	case EPowerupType::HealthRegeneration:
		EnableRegeneration();
		return;
	}
}

void UPowerupComponent::EnableSpeedBoost()
{
	OwningCharacter->GetCharacterMovement()->MaxWalkSpeed = 1600.0f;
	GetWorld()->GetTimerManager().SetTimer(PowerupTimer, this, &UPowerupComponent::DisableSpeedBoost, SpeedBoostDuration, false);
}

void UPowerupComponent::EnableDamageBoost()
{
	OwningCharacter->EnableDamageBoost();
	GetWorld()->GetTimerManager().SetTimer(PowerupTimer, this, &UPowerupComponent::DisableDamageBoost, DamageBoostDuration, false);
}

void UPowerupComponent::EnableRegeneration()
{
	OwningCharacter->GetHealthComponent()->StartRegenerateHealthTimer(RegenerationDuration);
	GetWorld()->GetTimerManager().SetTimer(PowerupTimer, this, &UPowerupComponent::DisableRegeneration, static_cast<float>(RegenerationDuration), false);
}

void UPowerupComponent::DisableSpeedBoost()
{
	bIsPowerupEnabled = false;
	OwningCharacter->GetCharacterMovement()->MaxWalkSpeed = 800.0f;
	UE_LOG(LogTemp, Warning, TEXT("SpeedBoost disabled"))
}

void UPowerupComponent::DisableDamageBoost()
{
	bIsPowerupEnabled = false;
	OwningCharacter->DisableDamageBoost();
	UE_LOG(LogTemp, Warning, TEXT("DamageBoost disabled"))
}

void UPowerupComponent::DisableRegeneration()
{
	UE_LOG(LogTemp, Warning, TEXT("Regeneration disabled"))
	bIsPowerupEnabled = false;
}

