// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerController.generated.h"

class AFPSCharacter;
class UScoreHandlingComponent;
class UControllerComponentsContainer;

UCLASS()
class FPS_API AFPSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AFPSPlayerController();
	FORCEINLINE UControllerComponentsContainer* GetControllerComponentsContainer() const { return ContrCompContainer; };
	void SpawnAndPossesSpectator();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowScoreboard();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HideScoreboard();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:

	virtual void BeginPlay() override;

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UControllerComponentsContainer* ContrCompContainer;	
	void SetMouseSensitivity();
};
