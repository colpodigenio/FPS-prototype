// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSGameMode.h"
#include "FPS.h"
#include "DeathmatchGameMode.generated.h"

UCLASS()
class FPS_API ADeathmatchGameMode : public AFPSGameMode
{
	GENERATED_BODY()

public:

	ADeathmatchGameMode();
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FString, FDeathmatchScore> ScoreBoard;
	FORCEINLINE int32 GetPointsToWin() const { return PointsToWin; };
	void EndMatch(FString WinnerName);
	void ReturnToLobby();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Rules", meta = (AllowPrivateAccess = "true"))
	int32 PointsToWin;
};
