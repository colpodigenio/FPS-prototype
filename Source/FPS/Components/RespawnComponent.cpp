// Fill out your copyright notice in the Description page of Project Settings.

#include "RespawnComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "RespawnPoint.h"
#include "FPSCharacter.h"
#include "FPS.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Controller.h"

URespawnComponent::URespawnComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void URespawnComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URespawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URespawnComponent::StartRespawnTimer()
{
	FTimerHandle RespawnTimer;
	Cast<AActor>(GetOwner())->GetWorldTimerManager().SetTimer(RespawnTimer, this, &URespawnComponent::RespawnPlayer, 5.0f, false);
}

void URespawnComponent::SetPawnToSpawn(TSubclassOf<AFPSCharacter> ClassToSpawn)
{
	PawnToSpawn = ClassToSpawn;
}

void URespawnComponent::RespawnPlayer()
{
	// Should choose furthest from enemies not random??
	TArray<AActor*> RespawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARespawnPoint::StaticClass(), RespawnPoints);
	int PlayerStartIndex = UKismetMathLibrary::RandomIntegerInRange(0, RespawnPoints.Num() - 1);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ARespawnPoint* RespawnPoint = Cast<ARespawnPoint>(RespawnPoints[PlayerStartIndex]);
	TSet<AActor*> OverlappingActors;
	RespawnPoint->GetCapsuleComponent()->GetOverlappingActors(OverlappingActors, AFPSCharacter::StaticClass());
	if (OverlappingActors.Num() > 0)
	{
		for (auto OverlappingActor : OverlappingActors)
		{
			Cast<AFPSCharacter>(OverlappingActor)->ReceiveDamage(1000, Cast<AController>(GetOwner()));
		}
	}
	AFPSCharacter* NewPawn = GetWorld()->SpawnActor<AFPSCharacter>(UGameplayStatics::GetGameMode(GetOwner())->DefaultPawnClass, RespawnPoints[PlayerStartIndex]->GetActorTransform(), SpawnParams);
	Cast<AController>(GetOwner())->Possess(NewPawn);
}

