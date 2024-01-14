// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/CharacterTypes.h"
#include "ElementSlotWidget.generated.h"

class UWidgetSwitcher;

UCLASS()
class ELEMENT_API UElementSlotWidget : public UUserWidget
{
	GENERATED_BODY()

	/*
	* 
	*/
public:
	FORCEINLINE UWidgetSwitcher* GetElementSwitcher() { return ElementSwitcher; }
	void SetElementSlot(EPlayerElement Element);

private:
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* ElementSwitcher;
	
};
