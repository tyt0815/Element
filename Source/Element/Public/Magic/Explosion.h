// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Magic/BaseMagicProjectile.h"
#include "Explosion.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENT_API AExplosion : public ABaseMagicProjectile
{
	GENERATED_BODY()

public:
	virtual void BeginBoxOverlapExec(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
private:
	void Explosion();

	UPROPERTY(EditAnywhere, Category = Attribute)
	float ExplosionDamageCoefficient = 1.0f;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float ExplosionRadius = 10.0f;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float ExplosionDelay = 0.1f;
};
