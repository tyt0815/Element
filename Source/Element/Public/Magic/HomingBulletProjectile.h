// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Magic/BaseMagicProjectile.h"
#include "HomingBulletProjectile.generated.h"

class USphereComponent;

UCLASS()
class ELEMENT_API AHomingBulletProjectile : public ABaseMagicProjectile
{
	GENERATED_BODY()

public:
	AHomingBulletProjectile();

public:
	void SetHomingTarget(AActor* Actor);

protected:
	virtual void BeginBoxOverlapExec(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
