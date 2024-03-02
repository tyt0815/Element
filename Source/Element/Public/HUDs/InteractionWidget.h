// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

class UTextBlock;

UCLASS()
class ELEMENT_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetInteractionHint(FString Hint);

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractionKey;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractionHint;
};
