// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPS.h"
#include "Components/ActorComponent.h"
#include "ScoreHandlingComponent.generated.h"

class AFPSGameMode;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API UScoreHandlingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UScoreHandlingComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly)
	FDeathmatchScore Score;
	UPROPERTY(EditDefaultsOnly)
	FName PlayerName;
	void SetPlayerNameInGM();
	void AddFrag();
	void AddDeath();
	void AddSuicide();

protected:
	virtual void BeginPlay() override;

private:
	void SendScoreToGM();
	TWeakObjectPtr<AFPSGameMode> GM;
};
