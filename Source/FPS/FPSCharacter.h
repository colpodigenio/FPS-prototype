// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPS.h"
#include "FPSCharacter.generated.h"

class UHealthComponent;
class UCameraComponent;
class AWeapon;
class USkeletalMeshComponent;
class UPowerupComponent;

UENUM(BlueprintType)
enum class EBotDifficulty : uint8
{
	Easy,
	Medium,
	Hard
};

UCLASS()
class FPS_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AFPSCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void ReceiveDamage(int32 DamageAmount);
	bool CheckIfCharacterHasWeapon(EWeaponType WeaponType);
	void AddAmmoFromWeaponPickup(EWeaponType WeaponType);
	UFUNCTION(BlueprintImplementableEvent)
	AWeapon* SpawnWeaponFromPickup(EWeaponType WeaponType);
	void AddWeaponFromWeaponPickup(EWeaponType WeaponType);
	FVector GetFPSCameraLocation() const;
	FVector GetFPSCameraForwardVector() const;
	bool IsPowerupEnabled() const;
	FORCEINLINE UHealthComponent* GetHealthComponent() const { return HealthComponent; };
	void EnablePowerup(EPowerupType PowerupType);
	UFUNCTION(BlueprintPure)
	AWeapon* GetWeaponByType(EWeaponType WeaponType);
	int32 GetHealth() const;
	bool CheckIfHealthIsFull();
	int32 GetArmor() const;
	bool CheckiIfArmorIsFull();
	UFUNCTION(BlueprintCallable)
	bool CheckiIfAmmoRanOut();
	void EnableDamageBoost();
	void DisableDamageBoost();

	const float RunningMultiplier = 0.55; // walking is 2 times slower

protected:

	virtual void BeginPlay() override;
	UFUNCTION(BlueprintImplementableEvent)
	void ShowScore();
	UFUNCTION(BlueprintImplementableEvent)
	void HideScore();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPowerupComponent* PowerupComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FPSCamera;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FName InventorySocketName;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AWeapon* CurrentWeapon;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<EWeaponType, AWeapon*> WeaponInventory;

	float Stamina;	// time in seconds which character can sprint
	float StaminaMax;
	bool bIsSprinting;
	bool bIsMoving;
	bool bIsMovingForward;
	bool bIsStrafing;
	bool bIsRunning;
	float MovementMultiplier;
	float LastMovementMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EBotDifficulty BotDifficulty;

	FVector LastLocation; // TO DELETE temporary variable to determine movement speed for debug

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
	void TakeNewWeapon(EWeaponType NewWeaponType);
	void TakeAssaultRifle();
	void TakeShotgun();
	void TakeRocketLauncher();
};
