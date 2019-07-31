// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSCharacter.h"
#include "Components/HealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"


AFPSCharacter::AFPSCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.DoNotCreateDefaultSubobject(AFPSCharacter::MeshComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCamera->SetupAttachment(RootComponent);
}

void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move", this, &AFPSCharacter::Move);
	PlayerInputComponent->BindAxis("Strafe", this, &AFPSCharacter::Strafe);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::Jump);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AFPSCharacter::Crouch);
	PlayerInputComponent->BindAction("FireWeapon", IE_Pressed, this, &AFPSCharacter::FireWeapon);
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
	AddMovementInput(GetCapsuleComponent()->GetForwardVector(), AxisValue);
}

void AFPSCharacter::Strafe(float AxisValue)
{
	AddMovementInput(GetCapsuleComponent()->GetRightVector(), AxisValue);
}

void AFPSCharacter::Jump()
{
	UE_LOG(LogTemp, Warning, TEXT("Jump"))
}

void AFPSCharacter::Crouch()
{
	UE_LOG(LogTemp, Warning, TEXT("Crouch"))
}

void AFPSCharacter::FireWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("FireWeapon"))
}

void AFPSCharacter::StartSprint()
{
	UE_LOG(LogTemp, Warning, TEXT("StartSprint"))
}

void AFPSCharacter::EndSprint()
{
	UE_LOG(LogTemp, Warning, TEXT("EndSprint"))
}

void AFPSCharacter::ToggleWalkRun()
{
	UE_LOG(LogTemp, Warning, TEXT("ToggleWalkRun"))
}

void AFPSCharacter::Zoom()
{
	UE_LOG(LogTemp, Warning, TEXT("Zoom"))
}

void AFPSCharacter::ReloadWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("ReloadWeapon"))
}

void AFPSCharacter::UseExplosive()
{
	UE_LOG(LogTemp, Warning, TEXT("UseExplosive"))
}

void AFPSCharacter::ShowScore()
{
	UE_LOG(LogTemp, Warning, TEXT("ShowScore"))
}
