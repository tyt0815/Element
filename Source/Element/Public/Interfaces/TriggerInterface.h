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

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Trigger)
	void InitTriggerInterface(UPARAM(ref) TArray<AActor*>& TriggerTargets);
	virtual void InitTriggerInterface_Implementation(TArray<AActor*>& TriggerTargets);

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Trigger)
	void Trigger(bool Condition, AActor* TriggeringActor, UPARAM(ref) TArray<AActor*>& TriggerTargets);
	virtual void Trigger_Implementation(bool Condition, AActor* TriggeringActor, TArray<AActor*>& TriggerTargets);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Trigger)
	bool IsTriggered() const;
	virtual bool IsTriggered_Implementation() const { return Triggered; }
	
protected:
	void SignalToTargets(TArray<AActor*>& TriggerTargets);
	void InitTriggerTargets(TArray<AActor*>& TriggerTargets);

	bool Triggered = false;

private:
	bool Initialized = false;
};
