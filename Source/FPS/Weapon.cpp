// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "UnrealMathUtility.h"
#include "FPSCharacter.h"
#include "Projectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"
#include "Kismet/KismetMathLibrary.h"

AWeapon::AWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.DoNotCreateDefaultSubobject(AWeapon::MeshComponentName))
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = WeaponMesh;
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(PICKUP_TRACE, ECR_Overlap);
	MuzzleFlashEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MuzzleFlashEffect"));
	MuzzleFlashEffect->SetupAttachment(GetWeaponMesh(), TEXT("Muzzle"));
	MuzzleFlashEffect->SetAutoActivate(false);
	ShotSoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("ShotSoundEffect"));
	ShotSoundEffect->SetupAttachment(GetWeaponMesh(), TEXT("Muzzle"));
	ShotSoundEffect->SetAutoActivate(false);
	WeaponTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RecoilTimeline"));

	DamageBooster = 3;
	bIsReloading = false;
	bFirstShotFired = false;
	WeaponType = EWeaponType::None;
	PickupType = EPickupType::Weapon;
}

void AWeapon::AddAmmo(int32 AmountOfMagazines)
{
	int32 NewAmmoTotal = AmmoTotal + AmountOfMagazines * AmmoMagazineCapacity;
	if (NewAmmoTotal > AmmoTotalCapacity)
		AmmoTotal = AmmoTotalCapacity;
	else
		AmmoTotal = NewAmmoTotal;
}

void AWeapon::StartFire()
{
	if ((AmmoTotal <= 0 && AmmoInMagazine <=0) || bIsReloading) return;
	if (AmmoInMagazine <= 0)
	{
		StartReload();
		return;
	}
	float ShotDelay;
	if (bFirstShotFired)
		ShotDelay = FMath::Max(1 / FireRate - (GetWorld()->TimeSeconds - LastShotTime), 0.0f);
	else
		ShotDelay = 0.0f;
	bFirstShotFired = true;
	GetWorldTimerManager().SetTimer(FireTimer, this, &AWeapon::Fire, 1 / FireRate, true, ShotDelay);
}

void AWeapon::RecoilTimelineCallback(float Value)
{
	AFPSCharacter* Player = Cast<AFPSCharacter>(GetOwner());
	RecoilValue = DefaultRecoilValue * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) / WeaponTimeline->GetTimelineLength();
	Player->AddControllerPitchInput(RecoilValue);
}

void AWeapon::RecoilTimelineFinish()
{
}

void AWeapon::Fire()
{
	LastShotTime = GetWorld()->TimeSeconds;
	ShotProjectile();
	AddRecoil();
	if (MuzzleFlashEffect)
		MuzzleFlashEffect->ActivateSystem();
	if (ShotSoundEffect)
		ShotSoundEffect->Activate(true);
	DecreaseAmmoAmount();
	UE_LOG(LogTemp, Warning, TEXT("%s has fired, ammo in magazine %i"), *GetName(), AmmoInMagazine)
}

void AWeapon::AddRecoil()
{
	if (WeaponTimeline)
		WeaponTimeline->PlayFromStart();
}

void AWeapon::ShotProjectile()
{
	// if (GetOwner()->GetController.GetClass() == APlayerController::StaticClass())
	UE_LOG(LogTemp, Warning, TEXT("%i"), DamageAmount)
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = this;
	FVector NoiseLocation = UKismetMathLibrary::RandomPointInBoundingBox(GetWeaponMesh()->GetSocketLocation(TEXT("Muzzle")), FVector(400.0f, 400.0f, 50.0f));
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), NoiseLocation, 1.f, GetOwner(), 5000.0f);
	ensureMsgf(SpawnParams.Owner, TEXT("When shooting this projectile you should set the firing weapon as an owner in FActorSpawnParameters"));
	GetWorld()->SpawnActor<AProjectile>(ProjectileType, GetWeaponMesh()->GetSocketTransform(TEXT("Muzzle")), SpawnParams);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	DefaultDamage = DamageAmount;

	ensureMsgf(RecoilCurve, TEXT("You should choose curve for %s"), *GetName());
	if (RecoilCurve)
	{
		OnRecoilTimelineCallback.BindDynamic(this, &AWeapon::RecoilTimelineCallback);
		OnRecoilTimelineFinish.BindUObject(this, &AWeapon::RecoilTimelineFinish);
		WeaponTimeline->AddInterpFloat(RecoilCurve, OnRecoilTimelineCallback);
		WeaponTimeline->SetTimelineFinishedFunc(OnRecoilTimelineFinish);
		WeaponTimeline->SetLooping(false);
		WeaponTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
	}
}

void AWeapon::DecreaseAmmoAmount()
{
	AmmoInMagazine--;
	if (AmmoInMagazine <= 0)
		StopFire();
}

void AWeapon::StopFire()
{
	if(GetWorldTimerManager().IsTimerActive(FireTimer))
		GetWorldTimerManager().ClearTimer(FireTimer);
}

void AWeapon::Aim()
{
	UE_LOG(LogTemp, Warning, TEXT("%s Aims"), *GetName())
}

void AWeapon::StartReload()
{
	if (bIsReloading || AmmoTotal == 0 || AmmoInMagazine == AmmoMagazineCapacity) return;
	FTimerHandle ReloadTimer;
	GetWorldTimerManager().SetTimer(ReloadTimer, this, &AWeapon::Reload, ReloadTime, false);
	bIsReloading = true;
}

void AWeapon::Reload()
{
	int32 DeltaAmmoInMagazine = AmmoMagazineCapacity - AmmoInMagazine;
	int32 NewAmmoTotal = AmmoTotal - DeltaAmmoInMagazine;
	if (NewAmmoTotal < 0)
	{
		AmmoInMagazine += DeltaAmmoInMagazine + NewAmmoTotal;
		AmmoTotal = 0;
	}
	else
	{
		AmmoInMagazine = AmmoMagazineCapacity;
		AmmoTotal -= DeltaAmmoInMagazine;
	}
	UE_LOG(LogTemp, Warning, TEXT("%s has been reloaded. %i bullets in magazine %i bullets total"), *GetName(), AmmoInMagazine, AmmoTotal)
	bIsReloading = false;
}

bool AWeapon::TryApplyToCharacter(AFPSCharacter* Character)
{
	if (Character->CheckIfCharacterHasWeapon(WeaponType) && Character->GetWeaponByType(WeaponType)->CheckIfAmmoIsFull())
		return false;
	Character->CheckIfCharacterHasWeapon(WeaponType) ? Character->AddAmmoFromWeaponPickup(WeaponType) :	Character->AddWeaponFromWeaponPickup(WeaponType);
	return true;
}

bool AWeapon::CheckIfAmmoIsFull()
{
	if (AmmoTotal == AmmoTotalCapacity)
		return true;
	else
		return false;
}

bool AWeapon::CheckIfAmmoRanOut()
{
	if (AmmoTotal == 0 && AmmoInMagazine == 0)
		return true;
	else
		return false;
}

void AWeapon::ShowMesh()
{
	Super::ShowMesh();
	GetWeaponMesh()->SetVisibility(true);
}

void AWeapon::HideMesh()
{
	Super::HideMesh();
	GetWeaponMesh()->SetVisibility(false);
}

void AWeapon::ChangeDamage(int32 DamageBooster)
{
	DamageAmount = DamageBooster * DefaultDamage;
}
