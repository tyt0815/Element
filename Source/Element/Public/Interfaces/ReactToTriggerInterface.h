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
	void ReactToTrigger();

	UFUNCTION(BlueprintNativeEvent)
	void AddTrigger(AActor* Trigger);
	virtual void AddTrigger_Implementation(AActor* Trigger) { Triggers.Add(Trigger); }

protected:
	TArray<AActor*> Triggers;
};
