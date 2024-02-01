// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Magic/BaseMagicProjectile.h"
#include "Piercing.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENT_API APiercing : public ABaseMagicProjectile
{
	GENERATED_BODY()

public:
	void SetPiercingDelay(float Value) { PiercingDelay = Value; }

protected:
	virtual void BeginBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void PiercingDamage();

	float PiercingDelay = 0.1;
	FTimerHandle PiercingTimer;
};
