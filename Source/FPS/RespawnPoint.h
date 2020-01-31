// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RespawnPoint.generated.h"

class UCapsuleComponent;

UCLASS()
class FPS_API ARespawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ARespawnPoint();
	FORCEINLINE UCapsuleComponent* GetCapsuleComponent() const { return CapsuleComponent; };
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;
};
