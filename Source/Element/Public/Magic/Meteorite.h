// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Magic/BaseMagicProjectile.h"
#include "Meteorite.generated.h"

class USphereComponent;
class UCameraShakeBase;
class UCameraShakeSourceComponent;

UCLASS()
class ELEMENT_API AMeteorite : public ABaseMagicProjectile
{
	GENERATED_BODY()

public:
	AMeteorite();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:

protected:
	virtual void MultiStageHit(float Damage) override;
	virtual void EndMagic() override;
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere)
	float MSHDelay = 0.1f;

	UPROPERTY(VisibleAnywhere)
	UCameraShakeSourceComponent* CameraShakeComponent;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float ExplosionCoefficient = 1.0f;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float ExplosionRadiusRate = 1.2f;

	UPROPERTY(EditAnywhere, Category = Attribute)
	TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float ExplosionCameraShakeCoefficient = 4.0f;
};
