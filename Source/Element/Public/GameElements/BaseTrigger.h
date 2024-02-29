#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/TriggerInterface.h"
#include "BaseTrigger.generated.h"

UCLASS()
class ELEMENT_API ABaseTrigger : public AActor , public ITriggerInterface
{
	GENERATED_BODY()
	
public:	
	ABaseTrigger();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Trigger_Implementation(AActor* TriggeringgActor) override;

protected:
	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> TriggerTargets;
};
