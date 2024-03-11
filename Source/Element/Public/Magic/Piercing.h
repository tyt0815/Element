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
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float PiercingDelay = 0.1f;
};
