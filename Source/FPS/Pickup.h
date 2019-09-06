// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

class AFPSCharacter;
class UStaticMeshComponent;

UCLASS()
class FPS_API APickup : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

public:	
	APickup(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void Tick(float DeltaTime) override;	
	virtual bool TryApplyToCharacter(AFPSCharacter* Character);
	virtual void ShowMesh();
	virtual void HideMesh();


protected:
	virtual void BeginPlay() override;	
	static FName MeshComponentName;
};
