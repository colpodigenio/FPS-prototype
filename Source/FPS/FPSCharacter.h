// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

class UHealthComponent;
class UCameraComponent;
class AWeapon;

UCLASS()
class FPS_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FPSCamera;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AWeapon* CurrentWeapon;

	float Stamina;	// time in seconds which character can sprint
	float StaminaMax; 
	bool bIsSprinting;
	bool bIsMoving;
	bool bIsMovingForward;
	bool bIsStrafing;
	bool bIsRunning;
	const float RunningMultiplier = 0.55; // walking is 2 times slower
	float MovementMultiplier;
	float LastMovementMultiplier;

	FVector LastLocation; // TO DELETE temporary variable to determine movement speed for debug

public:
	AFPSCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	void Move(float AxisValue);
	void Strafe(float AxisValue);
	void NormalizeMoveStrafeVector(float &Multiplier); // this function should be called in both Move() and Strafe() function to prevent increased speed while moving and strafing same time
	void TryJump();
	void PerformCrouch();	// maybe should be changed in future to implement it smoothly with animation. and maybe implemented as toggle
	void PerformUnCrouch();
	void StartFiringWeapon();
	void StopFiringWeapon();
	void StartSprint();
	void Sprint(float DeltaTime);
	void RestoreStamina(float DeltaTime);
	void EndSprint();
	void ToggleWalkRun();
	void Zoom();
	void ReloadWeapon();
	void UseExplosive();

protected:

	UFUNCTION(BlueprintImplementableEvent)
	void ShowScore();
};
