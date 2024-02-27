// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TriggerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTriggerInterface : public UInterface
{
	GENERATED_BODY()
};

class ELEMENT_API ITriggerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Trigger)
	void Trigger();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Trigger)
	void Halt();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Trigger)
	bool IsTriggered() const;
	bool IsTriggered_Implementation() const { return Triggered; }
	
protected:
	bool Triggered;
};
