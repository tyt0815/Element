// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Magic/BaseMagic.h"
#include "Summon.generated.h"

class USphereComponent;

UCLASS()
class ELEMENT_API ASummon : public ABaseMagic
{
	GENERATED_BODY()

public:
	ASummon();
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE void SetLocationRelatedWithOwner(FVector V) { LocationRelatedWithOwner = V; }

protected:
	virtual void DetectTarget();
	virtual void BeginPlay() override;
	FVector GetWorldLocationRelatedWithOwner();

	UPROPERTY(VisibleAnywhere)
	USphereComponent* DetectionSphereComponent;

private:
	void Attack();

	UPROPERTY(EditAnywhere, Category = Attribute)
	float MinRotateSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float MaxRotateSpeed = 2.0f;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float FollowingLerpAlpha = 0.01f;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float AcceptanceRadius = 30.0f;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float AttackDelay = 0.1;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float LifeTime = 10.0f;

	UPROPERTY(EditAnywhere, Category = Attribute)
	TSubclassOf<class AHomingBulletProjectile> BulletClass;

	UPROPERTY(EditAnywhere, Category = Attribute)
	UMaterialInterface* OnMaterial;

	UPROPERTY(EditAnywhere, Category = Attribute)
	UMaterialInterface* OffMaterial;
	
	
	FRotator TickRotator;
	float RotationSpeed;
	FVector LocationRelatedWithOwner;
	FTimerHandle AttackTimer;

	AActor* Target;
};
