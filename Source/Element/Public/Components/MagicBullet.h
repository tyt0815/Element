// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BaseAbilityComponent.h"
#include "MagicBullet.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENT_API UMagicBullet : public UBaseAbilityComponent
{
	GENERATED_BODY()
	
	virtual void OnStarted_Implementation(const FInputActionInstance& InputInstance) override;
};
