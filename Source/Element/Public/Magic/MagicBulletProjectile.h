// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Magic/BaseMagicProjectile.h"
#include "MagicBulletProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENT_API AMagicBulletProjectile : public ABaseMagicProjectile
{
	GENERATED_BODY()
protected:
	virtual void BeginBoxOverlapExec(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
