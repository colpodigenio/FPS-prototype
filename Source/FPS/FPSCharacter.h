// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

class UHealthComponent;

UCLASS()
class FPS_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

public:
	AFPSCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	//void Move(float AxisValue);
	//void Strafe(float AxisValue);
	//void Jump(); //how to implement second jump??
	//void Crouch();
	//void ToggleWalkRun();
	//void Sprint();
	//void FireWeapon() //executes interface function
	//void AimWeapon() //executes interface function
	//void ReloadWeapon() //executes interface function
	//void UseExplosive() //or ThrowExplosive??
};
