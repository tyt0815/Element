// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ELEMENT_API IInteractionInterface
{
	GENERATED_BODY()

	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interaction)
	void Interact(AActor* InteractingActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interaction)
	bool IsInteractable() const;
	bool IsInteractable_Implementation() const { return Interactable; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interaction)
	FString GetInteractionHint() const;
	virtual FString GetInteractionHint_Implementation() const { return FString("Interaction"); }

protected:
	bool Interactable;
};
