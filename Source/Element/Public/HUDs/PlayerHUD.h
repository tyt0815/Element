// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UPlayerOverlay;

UCLASS()
class ELEMENT_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

	/*
	* HUD ±âº»
	*/
public:
	FORCEINLINE UPlayerOverlay* GetPlayerOverlay() { return PlayerOverlay; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "ElementPlayer")
	TSubclassOf<UPlayerOverlay> PlayerOverlayClass;

	UPROPERTY()
	UPlayerOverlay* PlayerOverlay;
};
