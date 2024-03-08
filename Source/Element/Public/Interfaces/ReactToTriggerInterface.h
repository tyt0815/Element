#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ReactToTriggerInterface.generated.h"


UINTERFACE(MinimalAPI)
class UReactToTriggerInterface : public UInterface
{
	GENERATED_BODY()
};


class ELEMENT_API IReactToTriggerInterface
{
	GENERATED_BODY()

	
public:
	UFUNCTION(BlueprintNativeEvent)
	void ReactToTrigger(AActor* Trigger);

	UFUNCTION(BlueprintNativeEvent)
	void AddTrigger(AActor* Trigger);
	virtual void AddTrigger_Implementation(AActor* Trigger) { Triggers.Add(Trigger); }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IsAllTriggersTriggered(bool& Condition);
	virtual void IsAllTriggersTriggered_Implementation(bool& Condition);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IsAllTriggersTriggerOff(bool& Condition);
	virtual void IsAllTriggersTriggerOff_Implementation(bool& Condition);

private:
	TArray<AActor*> Triggers;
};
