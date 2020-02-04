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
	void SetPlayerNameInGM();
	void AddFrag();
	void AddDeath();
	void AddSuicide();
	void SetPlayerName(FName NewName);

protected:
	virtual void BeginPlay() override;

private:
	FName PlayerName;
	FPlayerProfileData ProfileData;
	void SendScoreToGM();
	void CalculateScore(int32 ScoreDelta);
	TWeakObjectPtr<AFPSGameMode> GM;
};
