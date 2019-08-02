// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSCharacter.h"
#include "Components/HealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WeaponInterface.h"
#include "Weapon.h"
#include "Kismet/KismetMathLibrary.h"

AFPSCharacter::AFPSCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.DoNotCreateDefaultSubobject(AFPSCharacter::MeshComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCamera->SetupAttachment(RootComponent);

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	JumpMaxCount = 2;

	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 100.f;
	StaminaMax = 5.0f;
	Stamina = StaminaMax;
	MovementMultiplier = 0.55f;
	LastMovementMultiplier = MovementMultiplier;

	bIsSprinting = false;
	bIsMoving = false;
	bIsStrafing = false;
	bIsRunning = true;
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
	PlayerInputComponent->BindAction("ShowScore", IE_Pressed, this, &AFPSCharacter::ShowScore);

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
	if (Stamina > 0.25)
	{
		Jump();
		Stamina -= 0.25;
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
			Stamina += DeltaTime;
		else
			Stamina += 2 * DeltaTime;
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

