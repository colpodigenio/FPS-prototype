// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSCharacter.h"
#include "Components/HealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	MovementState = EMovementState::Running;
	LastMovingState = MovementState;
	GetCharacterMovement()->MaxWalkSpeed = MovementState;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 100.f;
	StaminaMax = 5.0f;
	Stamina = StaminaMax;
	bIsStrafing = false;
	bIsSprinting = false;
	bIsNotMovingBackwards = true;
	bIsZooming = false;

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

	Zoom();
	Sprint(DeltaTime);
	RestoreStamina(DeltaTime);

	auto Location = GetActorLocation();
	float Speed = (Location - LastLocation).Size();
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
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AFPSCharacter::StartZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AFPSCharacter::EndZoom);
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
	AddMovementInput(GetCapsuleComponent()->GetForwardVector(), AxisValue);
	AxisValue >= 0.0f ? bIsNotMovingBackwards = true : bIsNotMovingBackwards = false;
}

void AFPSCharacter::Strafe(float AxisValue)
{
	if (bIsSprinting) return;
	AddMovementInput(GetCapsuleComponent()->GetRightVector(), AxisValue);
	AxisValue == 0.0f ? bIsStrafing = false : bIsStrafing = true;		
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
}

void AFPSCharacter::PerformUnCrouch()
{
	UnCrouch();
}

void AFPSCharacter::StartSprint()
{
	if (!bIsStrafing && bIsNotMovingBackwards && Stamina > 0.0f)
	{
		MovementState = EMovementState::Sprinting;
		GetCharacterMovement()->MaxWalkSpeed = MovementState;
	}
}

void AFPSCharacter::Sprint(float DeltaTime)
{
	if (GetCharacterMovement()->IsFalling() || Stamina <= 0.0f)
		EndSprint();
	if (MovementState == EMovementState::Sprinting && Stamina > 0)
		Stamina -= DeltaTime;
}

void AFPSCharacter::RestoreStamina(float DeltaTime)
{
	if (GetCharacterMovement()->IsFalling())
		return;
	if (Stamina < StaminaMax)
	{
		switch (MovementState)
		{
		case EMovementState::Walking:
			Stamina += DeltaTime / 2.0f;
			break;
		case EMovementState::Running:
			Stamina += DeltaTime / 4.0f;
			break;
		}
	}
	else
		Stamina = StaminaMax;
}

void AFPSCharacter::EndSprint()
{
	MovementState = LastMovingState;
	GetCharacterMovement()->MaxWalkSpeed = MovementState;
}

void AFPSCharacter::ToggleWalkRun()
{
	if (bIsCrouched) return;
	switch (MovementState)
	{
	case EMovementState::Walking:
		MovementState = EMovementState::Running;
		LastMovingState = MovementState;
		GetCharacterMovement()->MaxWalkSpeed = MovementState;
		break;
	case EMovementState::Running:
		MovementState = EMovementState::Walking;
		LastMovingState = MovementState;
		GetCharacterMovement()->MaxWalkSpeed = MovementState;
		break;
	}
}

void AFPSCharacter::StartZoom()
{
	bIsZooming = true;
}

void AFPSCharacter::Zoom()
{
	float TargetFoV = bIsZooming ? 60.0f : 90.0f;
	float NewFoV = UKismetMathLibrary::FInterpTo(FPSCamera->FieldOfView, TargetFoV, GetWorld()->GetDeltaSeconds(), 15.0f);
	FPSCamera->FieldOfView = NewFoV;
}

void AFPSCharacter::EndZoom()
{
	bIsZooming = false;
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

void AFPSCharacter::ReceiveDamage(int32 DamageAmount, AController* DamageInstigator)
{
	HealthComponent->ApplyDamage(DamageAmount, DamageInstigator);
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

FRotator AFPSCharacter::GetFPSCameraRotation() const
{
	return FPSCamera->GetComponentRotation();
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