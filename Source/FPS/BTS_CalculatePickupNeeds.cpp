// Fill out your copyright notice in the Description page of Project Settings.

#include "BTS_CalculatePickupNeeds.h"
#include "FPSCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Weapon.h"

UBTS_CalculatePickupNeeds::UBTS_CalculatePickupNeeds()
{
	bNotifyBecomeRelevant = true;
}

void UBTS_CalculatePickupNeeds::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	OwnerPawn = Cast<AFPSCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	Blackboard = OwnerComp.GetBlackboardComponent();
	if (OwnerPawn.Get() && Blackboard.Get())
	{
		CalculateHealthNeed();
		CalculateArmorNeed();
		CalculateAssaultRifleAmmoAndWeaponNeed();
		CalculateShotgunAmmoAndWeaponNeed();
		CalculateRocketLauncherAmmoAndWeaponNeed();
		CalculatePowerupNeed();
	}
}

void UBTS_CalculatePickupNeeds::CalculateHealthNeed()
{
	if (OwnerPawn->GetHealth() < 70)
		Blackboard->SetValueAsFloat(HealthNeedRatio.SelectedKeyName, 2.0f);
	else if (OwnerPawn->GetHealth() >= 100)
		Blackboard->SetValueAsFloat(HealthNeedRatio.SelectedKeyName, 0.0f);
	else
		Blackboard->SetValueAsFloat(HealthNeedRatio.SelectedKeyName, 0.5f);
}

void UBTS_CalculatePickupNeeds::CalculateArmorNeed()
{
	if (OwnerPawn->GetArmor() < 50)
		Blackboard->SetValueAsFloat(ArmorNeedRatio.SelectedKeyName, 1.5f);
	else if (OwnerPawn->GetArmor() >= 100)
		Blackboard->SetValueAsFloat(ArmorNeedRatio.SelectedKeyName, 0.0f);
	else
		Blackboard->SetValueAsFloat(ArmorNeedRatio.SelectedKeyName, 0.5f);
}

void UBTS_CalculatePickupNeeds::CalculateAssaultRifleAmmoAndWeaponNeed()
{
	AWeapon* AssaultRifleRef = OwnerPawn->GetWeaponByType(EWeaponType::AssaultRifle);
	if (AssaultRifleRef)
	{
		Blackboard->SetValueAsFloat(AssaultRifleAmmoNeedRatio.SelectedKeyName, AssaultRifleRef->GetRelativeAmmoCapacity());
		Blackboard->SetValueAsFloat(AssaultRifleNeedRatio.SelectedKeyName, AssaultRifleRef->GetRelativeAmmoCapacity());
	}
	else
	{
		Blackboard->SetValueAsFloat(AssaultRifleAmmoNeedRatio.SelectedKeyName, 0.0f);
		Blackboard->SetValueAsFloat(AssaultRifleNeedRatio.SelectedKeyName, 1.0f);
	}
}

void UBTS_CalculatePickupNeeds::CalculateShotgunAmmoAndWeaponNeed()
{
	AWeapon* ShotgunRef = OwnerPawn->GetWeaponByType(EWeaponType::Shotgun);
	if (ShotgunRef)
	{
		Blackboard->SetValueAsFloat(ShotgunAmmoNeedRatio.SelectedKeyName, ShotgunRef->GetRelativeAmmoCapacity());
		Blackboard->SetValueAsFloat(ShotgunNeedRatio.SelectedKeyName, ShotgunRef->GetRelativeAmmoCapacity());
	}
	else
	{
		Blackboard->SetValueAsFloat(ShotgunAmmoNeedRatio.SelectedKeyName, 0.0f);
		Blackboard->SetValueAsFloat(ShotgunNeedRatio.SelectedKeyName, 1.0f);
	}
}

void UBTS_CalculatePickupNeeds::CalculateRocketLauncherAmmoAndWeaponNeed()
{
	AWeapon* RocketLauncherRef = OwnerPawn->GetWeaponByType(EWeaponType::AssaultRifle);
	if (RocketLauncherRef)
	{
		Blackboard->SetValueAsFloat(RocketLauncherAmmoNeedRatio.SelectedKeyName, RocketLauncherRef->GetRelativeAmmoCapacity());
		Blackboard->SetValueAsFloat(RocketLauncherNeedRatio.SelectedKeyName, RocketLauncherRef->GetRelativeAmmoCapacity());
	}
	else
	{
		Blackboard->SetValueAsFloat(RocketLauncherAmmoNeedRatio.SelectedKeyName, 0.0f);
		Blackboard->SetValueAsFloat(RocketLauncherNeedRatio.SelectedKeyName, 1.0f);
	}
}

void UBTS_CalculatePickupNeeds::CalculatePowerupNeed()
{
	if(OwnerPawn->IsPowerupEnabled())
		Blackboard->SetValueAsFloat(PowerupNeedRatio.SelectedKeyName, 0.0f);
	else
		Blackboard->SetValueAsFloat(PowerupNeedRatio.SelectedKeyName, 1.0f);
}

