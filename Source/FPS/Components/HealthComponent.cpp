// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "FPSCharacter.h"
#include "FPS.h"
#include "GameFramework/Controller.h"
#include "ScoreHandlingComponent.h"

UHealthComponent::UHealthComponent()
	:HealthMax(100), HealthMaxBoosted(200), Health(HealthMax), ArmorMax(100), Armor(25), bIsDead(false), bIsRegenerating(false), HealthRegenerationDelta(2), RegenerationRate(2.0f) {}

void UHealthComponent::ApplyHeal(int32 HealDelta)
{
	if (Health > 0)
	{
		int32 NewHealth = Health + HealDelta;
		if (Health <= HealthMax && NewHealth >= HealthMax)
			Health = HealthMax;
		else if (Health <= HealthMax && NewHealth < HealthMax)
			Health = NewHealth;
	}
}

void UHealthComponent::AddArmor(int32 ArmorDelta, bool CanBoostMaxArmor)
{
	if (Health > 0)
	{
		int32 NewArmor = Armor + ArmorDelta;
		if (NewArmor >= ArmorMax && !CanBoostMaxArmor)
			Armor = ArmorMax;
		else
			Armor = NewArmor;
	}
}

void UHealthComponent::ApplyDamage(int32 DamageDelta, AController* DamageInstigator)
{
	if (Armor > 0)
	{
		int32 NewArmor = Armor - 3 * DamageDelta / 4;
		int32 NewHealth = Health - DamageDelta / 4;
		if (NewArmor < 0)
		{
			Armor = 0;
			Health = NewHealth + NewArmor;
		}
		else
		{
			Armor = NewArmor;
			Health = NewHealth;
		}
	}
	else
		Health -= DamageDelta;
	if (Health <= 0)
	{
		if(DamageInstigator != Cast<APawn>(GetOwner())->GetController())
			Cast<UScoreHandlingComponent>(DamageInstigator->FindComponentByClass(UScoreHandlingComponent::StaticClass()))->AddFrag();
		else
			Cast<UScoreHandlingComponent>(DamageInstigator->FindComponentByClass(UScoreHandlingComponent::StaticClass()))->AddSuicide();
		Cast<UScoreHandlingComponent>(Cast<APawn>(GetOwner())->GetController()->FindComponentByClass(UScoreHandlingComponent::StaticClass()))->AddDeath();
		Cast<APawn>(GetOwner())->GetController()->UnPossess();
		Die();
	}
}

void UHealthComponent::StartRegenerateHealthTimer(int32 NewRegenerationDuration)
{
	RegenerationDuration = NewRegenerationDuration;
	bIsRegenerating = true;
	if (GetOwner())
		GetOwner()->GetWorldTimerManager().SetTimer(RegenerateHealthTimer, this, &UHealthComponent::RegenerateHealth, 1 / RegenerationRate, true);
}

void UHealthComponent::RegenerateHealth()
{
	Health += HealthRegenerationDelta;
	RegenerationDuration--;
	StopRevertHealthToMaxTimer();
	if (RegenerationDuration <= 0)
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(RegenerateHealthTimer);
		bIsRegenerating = false;
		StartRevertHealthToMaxTimer();
	}
}

void UHealthComponent::StartRevertHealthToMaxTimer()
{
	if (!bIsRegenerating && Health > HealthMax)
		if (GetOwner())
			GetOwner()->GetWorldTimerManager().SetTimer(RevertHealthToMaxTimer, this, &UHealthComponent::RevertHealthToMax, 1.0f, true);
}

void UHealthComponent::RevertHealthToMax()
{
	if (Health > HealthMax)
		Health--;
	else
		GetOwner()->GetWorldTimerManager().ClearTimer(RevertHealthToMaxTimer);
}

void UHealthComponent::StopRevertHealthToMaxTimer()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(RevertHealthToMaxTimer);
}

void UHealthComponent::Die()
{
	bIsDead = true;
	GetOwner()->Destroy();
}

