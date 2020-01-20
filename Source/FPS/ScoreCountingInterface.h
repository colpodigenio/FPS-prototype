// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ScoreCountingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UScoreCountingInterface : public UInterface
{
	GENERATED_BODY()
};

class FPS_API IScoreCountingInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	FName PlayerName;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetPlayerNameInGM();
// 	UFUNCTION()
// 	virtual void AddFrag();
// 	UFUNCTION()
// 	virtual void RemoveFrag();
// 	UFUNCTION()
// 	virtual void AddSuicide();
// 	UFUNCTION()
// 	virtual void CalculateScore();
};
