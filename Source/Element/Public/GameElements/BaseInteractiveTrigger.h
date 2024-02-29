#pragma once

#include "CoreMinimal.h"
#include "GameElements/BaseInteractiveActor.h"
#include "Interfaces/TriggerInterface.h"
#include "BaseInteractiveTrigger.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENT_API ABaseInteractiveTrigger : public ABaseInteractiveActor, public ITriggerInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void Interact_Implementation(AActor* InteractingActor) override;
	virtual void Trigger_Implementation(AActor* TriggeringgActor) override;

protected:
	//virtual void SetTriggerTargets() override {};

	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> TriggerTargets;
};
