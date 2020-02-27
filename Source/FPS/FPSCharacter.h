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

UCLASS()
class FPS_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AFPSCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void ReceiveDamage(int32 DamageAmount, AController* DamageInstigator);
	bool CheckIfCharacterHasWeapon(EWeaponType WeaponType);
	void AddAmmoFromWeaponPickup(EWeaponType WeaponType);
	UFUNCTION(BlueprintImplementableEvent)
	AWeapon* SpawnWeaponFromPickup(EWeaponType WeaponType);
	void AddWeaponFromWeaponPickup(EWeaponType WeaponType);
	FVector GetFPSCameraLocation() const;
	FVector GetFPSCameraForwardVector() const;
	FRotator GetFPSCameraRotation() const;
	bool IsPowerupEnabled() const;
	FORCEINLINE UHealthComponent* GetHealthComponent() const { return HealthComponent; };
	FORCEINLINE UCameraComponent* GetFPSCamera() const { return FPSCamera; };
	void EnablePowerup(EPowerupType PowerupType);
	UFUNCTION(BlueprintPure)
	AWeapon* GetWeaponByType(EWeaponType WeaponType);
	FORCEINLINE AWeapon* GetCurrentWeapon() const { return CurrentWeapon; };
	int32 GetHealth() const;
	bool CheckIfHealthIsFull();
	int32 GetArmor() const;
	bool CheckiIfArmorIsFull();
	UFUNCTION(BlueprintCallable)
	bool CheckiIfAmmoRanOut();
	void EnableDamageBoost();
	void DisableDamageBoost();
	void TakeNewWeapon(EWeaponType NewWeaponType);
	bool CheckIfWeaponByTypeIsEmpty(EWeaponType WeaponType);

	UFUNCTION(BlueprintCallable)
	void Move(float AxisValue = 1.0f);
	UFUNCTION(BlueprintCallable)
	void Strafe(float AxisValue = 1.0f);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Stamina;	// time in seconds which character can sprint
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float StaminaMax;
	bool bIsSprinting;
	bool bIsNotMovingBackwards;
	bool bIsZooming;
	bool bIsStrafing;
	EMovementState::Type MovementState;
	EMovementState::Type LastMovingState;

	FVector LastLocation; // TO DELETE temporary variable to determine movement speed for debug
	
	UFUNCTION(BlueprintCallable)
	void TryJump();
	void PerformCrouch();	//MAYBE SHOULD BE DELETED!!! maybe should be changed in future to implement it smoothly with animation. and maybe implemented as toggle 
	void PerformUnCrouch();
	void StartFiringWeapon();
	void StopFiringWeapon();
	void StartSprint();
	void Sprint(float DeltaTime);
	void RestoreStamina(float DeltaTime);
	void EndSprint();
	void ToggleWalkRun();
	void StartZoom();
	void Zoom();
	void EndZoom();
	void ReloadWeapon();
	void UseExplosive();
	void TakeAssaultRifle();
	void TakeShotgun();
	void TakeRocketLauncher();
};
