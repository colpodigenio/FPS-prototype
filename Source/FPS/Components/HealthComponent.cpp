// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
	:HealthMax(100), HealthMaxBoosted(200), Health(HealthMax), ArmorMax(200), Armor(25), bIsDead(false), bIsRegenerating(false) {}

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

void UHealthComponent::AddArmor(int32 ArmorDelta)
{
	if (Health > 0)
	{
		int32 NewArmor = Armor + ArmorDelta;
		if (NewArmor >= ArmorMax)
			Armor = ArmorMax;
		else
			Armor = NewArmor;
	}
}

void UHealthComponent::ApplyDamage(int32 DamageDelta)
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
		Die();
}

void UHealthComponent::StartRegenerateHealthTimer()
{
	RegenerationDuration = 30;
	bIsRegenerating = true;
	if (GetOwner())
		GetOwner()->GetWorldTimerManager().SetTimer(RegenerateHealthTimer, this, &UHealthComponent::RegenerateHealth, 1.0f, false);
}

void UHealthComponent::RegenerateHealth()
{
	Health++;
	RegenerationDuration--;
	if(RegenerationDuration <= 0)
		GetOwner()->GetWorldTimerManager().ClearTimer(RegenerateHealthTimer);
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

void UHealthComponent::Die()
{
	bIsDead = true;
}

