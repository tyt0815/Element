// Fill out your copyright notice in the Description page of Project Settings.


#include "Magic/Meteorite.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraShakeSourceComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Element/DebugMacro.h"

AMeteorite::AMeteorite()
{
	ProjectileMovementComponent->Velocity = FVector(0, 0, -1.0f);
	ProjectileMovementComponent->InitialSpeed = 500.0f;
	ProjectileMovementComponent->MaxSpeed = ProjectileMovementComponent->InitialSpeed;
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(GetRootComponent());
	CameraShakeComponent = CreateDefaultSubobject<UCameraShakeSourceComponent>(TEXT("CameraShakeSource"));
	CameraShakeComponent->SetupAttachment(GetRootComponent());
	CameraShakeComponent->Deactivate();
}

void AMeteorite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ProjectileRange - GetMovingDistance() < SphereCollision->GetScaledSphereRadius())
	{
		CameraShakeComponent->Activate();
		CameraShakeComponent->StartCameraShake(CameraShake);
	}
}

void AMeteorite::BeginPlay()
{
	Super::BeginPlay();

	SetMultiStageHit(GetOwnerATK() * DamageCoefficient, MSHDelay);
}

void AMeteorite::MultiStageHit(float Damage)
{
	InitActorsToIgnore();
	while (true)
	{
		FHitResult HitResult;
		UKismetSystemLibrary::SphereTraceSingleForObjects(
			this,
			GetActorLocation(),
			GetActorLocation(),
			SphereCollision->GetScaledSphereRadius(),
			BoxTraceObjectTypes,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			HitResult,
			true
		);
		if (HitResult.GetActor() == nullptr) break;
		ActorsToIgnore.Add(HitResult.GetActor());
		DamageActor(HitResult, Damage);
	}
}

void AMeteorite::EndMagic()
{
	CameraShakeComponent->StopAllCameraShakes();
	InitActorsToIgnore();
	while (true)
	{
		FHitResult HitResult;
		UKismetSystemLibrary::SphereTraceSingleForObjects(
			this,
			GetActorLocation(),
			GetActorLocation(),
			SphereCollision->GetScaledSphereRadius() * ExplosionRadiusRate,
			BoxTraceObjectTypes,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			HitResult,
			true
		);
		if (HitResult.GetActor() == nullptr) break;
		ActorsToIgnore.Add(HitResult.GetActor());
		DamageActor(HitResult, GetOwnerATK() * ExplosionCoefficient);
	}
	Destroy();
}
