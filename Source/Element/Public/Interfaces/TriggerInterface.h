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
	//ITriggerInterface();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Trigger)
	void Trigger(AActor* TriggeringActor);
	//virtual void Trigger_Implementation(AActor* TriggeringActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Trigger)
	void Halt(AActor* HaltingActor);
	//virtual void Halt_Implementation(AActor* HaltingActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Trigger)
	bool IsTriggered() const;
	bool IsTriggered_Implementation() const { return Triggered; }
	
protected:
	//void SignalToTargets();
	//void InitTriggerTargets();
	//virtual void SetTriggerTargets() = 0;

	bool Triggered;
	//TArray<AActor*> TriggerTargets;
};
