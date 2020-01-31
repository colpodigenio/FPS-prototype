// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ControllerComponentsContainer.generated.h"

class UScoreHandlingComponent;
class URespawnComponent;
class AFPSCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API UControllerComponentsContainer : public UActorComponent
{
	GENERATED_BODY()

public:	
	UControllerComponentsContainer();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FORCEINLINE URespawnComponent* GetRespawnComponent() const { return RespawnComp; };
	FORCEINLINE UScoreHandlingComponent* GetScoreHandlingComponent() const { return ScoreHandlingComp; };
	UPROPERTY(EditDefaultsOnly)
	FName PlayerName;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFPSCharacter> PawnToSpawn;

protected:
	virtual void BeginPlay() override;

private:
	   
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URespawnComponent* RespawnComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UScoreHandlingComponent* ScoreHandlingComp;
};
