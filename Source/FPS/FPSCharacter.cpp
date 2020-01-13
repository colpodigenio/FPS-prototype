// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSCharacter.h"
#include "Components/HealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WeaponInterface.h"
#include "Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "AssaultRifle.h"
#include "Components/SkeletalMeshComponent.h"
#include "Shotgun.h"
#include "RocketLauncher.h"
#include "Components/PowerupComponent.h"

AFPSCharacter::AFPSCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	PowerupComponent = CreateDefaultSubobject<UPowerupComponent>(TEXT("PowerupComponent"));
	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCamera->SetupAttachment(RootComponent);
	GetMesh()->SetupAttachment(FPSCamera);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionObjectType(ENEMY_OBJ);
	GetMesh()->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ENEMY_TRACE, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ENEMY_OBJ, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(PROJECTILE_OBJ, ECR_Block);
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	JumpMaxCount = 2;

	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 100.f;
	StaminaMax = 5.0f;
	Stamina = StaminaMax;
	MovementMultiplier = RunningMultiplier;
	LastMovementMultiplier = MovementMultiplier;

	bIsSprinting = false;
	bIsMoving = false;
	bIsStrafing = false;
	bIsRunning = true;

	InventorySocketName = TEXT("InventorySocket");

	WeaponInventory.Emplace(EWeaponType::AssaultRifle, nullptr);
	WeaponInventory.Emplace(EWeaponType::Shotgun, nullptr);
	WeaponInventory.Emplace(EWeaponType::RocketLauncher, nullptr);
}

void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();	
}

void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Sprint(DeltaTime);
	RestoreStamina(DeltaTime);

	auto Location = GetActorLocation();
	float Speed = (Location - LastLocation).Size();
	//UE_LOG(LogTemp, Warning, TEXT("Stamina  = %f"), Stamina)
	//UE_LOG(LogTemp, Warning, TEXT("Speed  = %f"), Speed/DeltaTime)
	LastLocation = GetActorLocation();
	
}

void AFPSCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move", this, &AFPSCharacter::Move);
	PlayerInputComponent->BindAxis("Strafe", this, &AFPSCharacter::Strafe);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::TryJump);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AFPSCharacter::PerformCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AFPSCharacter::PerformUnCrouch);
	PlayerInputComponent->BindAction("FireWeapon", IE_Pressed, this, &AFPSCharacter::StartFiringWeapon);
	PlayerInputComponent->BindAction("FireWeapon", IE_Released, this, &AFPSCharacter::StopFiringWeapon);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFPSCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFPSCharacter::EndSprint);
	PlayerInputComponent->BindAction("ToggleWalkRun", IE_Pressed, this, &AFPSCharacter::ToggleWalkRun);
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AFPSCharacter::Zoom);
	PlayerInputComponent->BindAction("ReloadWeapon", IE_Pressed, this, &AFPSCharacter::ReloadWeapon);
	PlayerInputComponent->BindAction("UseExplosive", IE_Pressed, this, &AFPSCharacter::UseExplosive);
	PlayerInputComponent->BindAction("ShowHideScore", IE_Pressed, this, &AFPSCharacter::ShowScore);
	PlayerInputComponent->BindAction("ShowHideScore", IE_Released, this, &AFPSCharacter::HideScore);
	PlayerInputComponent->BindAction("TakeAssaultRifle", IE_Pressed, this, &AFPSCharacter::TakeAssaultRifle);
	PlayerInputComponent->BindAction("TakeShotgun", IE_Pressed, this, &AFPSCharacter::TakeShotgun);
	PlayerInputComponent->BindAction("TakeRocketLauncher", IE_Pressed, this, &AFPSCharacter::TakeRocketLauncher);

}

void AFPSCharacter::Move(float AxisValue)
{
	float MoveValue = MovementMultiplier * AxisValue;
	NormalizeMoveStrafeVector(MoveValue);
	AddMovementInput(GetCapsuleComponent()->GetForwardVector(), MoveValue);
	AxisValue == 0.0f ? bIsMoving = false : bIsMoving = true;
	AxisValue > 0.0f ? bIsMovingForward = true : bIsMovingForward = false;
}

void AFPSCharacter::Strafe(float AxisValue)
{
	if (bIsSprinting) return;
	float StrafeValue = MovementMultiplier * AxisValue;
	NormalizeMoveStrafeVector(StrafeValue);
	AddMovementInput(GetCapsuleComponent()->GetRightVector(), StrafeValue);
	AxisValue == 0.0f ? bIsStrafing = false : bIsStrafing = true;		
}

void AFPSCharacter::NormalizeMoveStrafeVector(float &Multiplier)
{
	if (bIsMoving && bIsStrafing)
		Multiplier /= UKismetMathLibrary::Sqrt(2);
}

void AFPSCharacter::TryJump()
{
	if (Stamina > 0.5)
	{
		Jump();
		Stamina -= 0.5;
	}
}

void AFPSCharacter::PerformCrouch()
{
	Crouch();
	MovementMultiplier = 1.0f;
}

void AFPSCharacter::PerformUnCrouch()
{
	UnCrouch();
	MovementMultiplier = LastMovementMultiplier;
}

void AFPSCharacter::StartSprint()
{
	if (!bIsStrafing && bIsMovingForward && Stamina > 0.0f)
	{
		bIsSprinting = true;
		MovementMultiplier = 1.0f;
	}
}

void AFPSCharacter::Sprint(float DeltaTime)
{
	if (GetCharacterMovement()->IsFalling() || Stamina <= 0.0f)
		EndSprint();
	if (bIsSprinting && Stamina > 0)
		Stamina -= DeltaTime;
}

void AFPSCharacter::RestoreStamina(float DeltaTime)
{
	if (!bIsSprinting && !GetCharacterMovement()->IsFalling() && Stamina < StaminaMax)
	{
		if (bIsRunning)
			Stamina += DeltaTime / 4.0f;
		else
			Stamina += DeltaTime / 2.0f;
		if (Stamina > StaminaMax)
			Stamina = StaminaMax;
	}
}

void AFPSCharacter::EndSprint()
{
	bIsSprinting = false;
	MovementMultiplier = LastMovementMultiplier;
}

void AFPSCharacter::ToggleWalkRun()
{
	if (bIsCrouched) return;

	if (bIsRunning)
	{
		MovementMultiplier = 0.5f * RunningMultiplier;
		bIsRunning = false;
	}
	else
	{
		MovementMultiplier = RunningMultiplier;
		bIsRunning = true;
	}
	LastMovementMultiplier = MovementMultiplier;
}

void AFPSCharacter::StartFiringWeapon()
{
	if (CurrentWeapon)
		CurrentWeapon->StartFire();
}

void AFPSCharacter::StopFiringWeapon()
{
	if (CurrentWeapon)
		CurrentWeapon->StopFire();
}

void AFPSCharacter::Zoom()
{
	if (CurrentWeapon)
		CurrentWeapon->Aim();
}

void AFPSCharacter::ReloadWeapon()
{
	if (CurrentWeapon)
		CurrentWeapon->StartReload();
}

void AFPSCharacter::UseExplosive()
{
	UE_LOG(LogTemp, Warning, TEXT("UseExplosive"))
}

void AFPSCharacter::TakeNewWeapon(EWeaponType NewWeaponType)
{
	CurrentWeapon->HideMesh();
	CurrentWeapon = WeaponInventory.FindRef(NewWeaponType);
	CurrentWeapon->ShowMesh();
}

void AFPSCharacter::TakeAssaultRifle()
{
	if (WeaponInventory.FindRef(EWeaponType::AssaultRifle))
	{
		TakeNewWeapon(EWeaponType::AssaultRifle);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *CurrentWeapon->GetName())
	}
}

void AFPSCharacter::TakeShotgun()
{
	if (WeaponInventory.FindRef(EWeaponType::Shotgun))
	{
		TakeNewWeapon(EWeaponType::Shotgun);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *CurrentWeapon->GetName())
	}
}

void AFPSCharacter::TakeRocketLauncher()
{
	if (WeaponInventory.FindRef(EWeaponType::RocketLauncher))
	{
		TakeNewWeapon(EWeaponType::RocketLauncher);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *CurrentWeapon->GetName())
	}
}

bool AFPSCharacter::CheckIfWeaponByTypeIsEmpty(EWeaponType WeaponType)
{
	return GetWeaponByType(WeaponType)->CheckIfAmmoRanOut();
}

int32 AFPSCharacter::GetHealth() const
{
	return HealthComponent->GetHealth();
}

bool AFPSCharacter::CheckIfHealthIsFull()
{
	if (HealthComponent->GetHealth() >= HealthComponent->GetHealthMax())
		return true;
	else
		return false;
}

int32 AFPSCharacter::GetArmor() const
{
	return HealthComponent->GetArmor();
}

bool AFPSCharacter::CheckiIfArmorIsFull()
{
	if (HealthComponent->GetArmor() >= HealthComponent->GetArmorMax())
		return true;
	else
		return false;
}

bool AFPSCharacter::CheckiIfAmmoRanOut()
{
	bool bIsAssaultRifleEmpty = true, bIsShotgunEmpty = true, bIsRocketLauncherEmpty = true;
	if(GetWeaponByType(EWeaponType::AssaultRifle))
		bIsAssaultRifleEmpty = GetWeaponByType(EWeaponType::AssaultRifle)->CheckIfAmmoRanOut();
	if (GetWeaponByType(EWeaponType::Shotgun))
		bIsShotgunEmpty = GetWeaponByType(EWeaponType::Shotgun)->CheckIfAmmoRanOut();
	if (GetWeaponByType(EWeaponType::RocketLauncher))
		bIsRocketLauncherEmpty = GetWeaponByType(EWeaponType::RocketLauncher)->CheckIfAmmoRanOut();
	if (bIsAssaultRifleEmpty && bIsShotgunEmpty && bIsRocketLauncherEmpty)
		return true;
	else
		return false;
}

void AFPSCharacter::EnableDamageBoost()
{
	for (auto& Elem : WeaponInventory)
	{
		AWeapon* WeaponRef = Cast<AWeapon>(Elem.Value);
		if (WeaponRef)
			WeaponRef->ChangeDamage(WeaponRef->GetDamageBooster());
	}
}

void AFPSCharacter::DisableDamageBoost()
{
	for (auto& Elem : WeaponInventory)
	{
		AWeapon* WeaponRef = Cast<AWeapon>(Elem.Value);
		if (WeaponRef)
			WeaponRef->ChangeDamage(1);
	}
}

void AFPSCharacter::ReceiveDamage(int32 DamageAmount)
{
	HealthComponent->ApplyDamage(DamageAmount);
}

bool AFPSCharacter::CheckIfCharacterHasWeapon(EWeaponType WeaponType)
{
	if (WeaponInventory.FindRef(WeaponType))
		return true;
	else
		return false;
}

void AFPSCharacter::AddAmmoFromWeaponPickup(EWeaponType WeaponType)
{
	int32 AmountOfMagazinesToAdd = 2;
	WeaponInventory.FindRef(WeaponType)->AddAmmo(AmountOfMagazinesToAdd);
}

void AFPSCharacter::AddWeaponFromWeaponPickup(EWeaponType WeaponType)
{
	WeaponInventory.Emplace(WeaponType, SpawnWeaponFromPickup(WeaponType));
}

FVector AFPSCharacter::GetFPSCameraLocation() const
{
	return FPSCamera->GetComponentLocation();
}

FVector AFPSCharacter::GetFPSCameraForwardVector() const
{
	return FPSCamera->GetForwardVector();
}

bool AFPSCharacter::IsPowerupEnabled() const
{
	return PowerupComponent->IsPowerupEnabled();
}

void AFPSCharacter::EnablePowerup(EPowerupType PowerupType)
{
	PowerupComponent->EnablePowerup(PowerupType);
}

AWeapon* AFPSCharacter::GetWeaponByType(EWeaponType WeaponType)
{
	return WeaponInventory.FindRef(WeaponType);
}