// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPS.h"
#include "Pickup.generated.h"

class AFPSCharacter;
class UStaticMeshComponent;

UCLASS()
class FPS_API APickup : public AActor
{
	GENERATED_BODY()

public:	
	APickup(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void Tick(float DeltaTime) override;	
	virtual bool TryApplyToCharacter(AFPSCharacter* Character);
	virtual void ShowMesh();
	virtual void HideMesh();
	UFUNCTION(BlueprintPure)
	FORCEINLINE EPickupType GetPickupType() const { return PickupType; };
	FORCEINLINE bool IsPickupActive() const { return bIsPickupActive; };
	FORCEINLINE bool GetPickupValue() const { return PickupValue; };

	UPROPERTY(BlueprintReadOnly)
	int32 PickupID;

protected:
	virtual void BeginPlay() override;	
	static FName MeshComponentName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BlueprintProtected = "true"))
	EPickupType PickupType;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsPickupActive;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float PickupValue;
};
