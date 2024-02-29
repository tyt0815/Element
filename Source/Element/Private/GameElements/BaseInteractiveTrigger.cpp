#include "GameElements/BaseInteractiveTrigger.h"

#include "Interfaces/ReactToTriggerInterface.h"

void ABaseInteractiveTrigger::BeginPlay()
{
	Super::BeginPlay();

	for (auto TriggerTarget : TriggerTargets)
	{
		IReactToTriggerInterface* ReactTarget = Cast<IReactToTriggerInterface>(TriggerTarget);
		if (ReactTarget)
		{
			ReactTarget->Execute_AddTrigger(TriggerTarget, this);
		}
	}
}

void ABaseInteractiveTrigger::Interact_Implementation(AActor* InteractingActor)
{
	Execute_Trigger(this, this);
}

void ABaseInteractiveTrigger::Trigger_Implementation(AActor* TriggeringActor)
{
	Triggered = !Triggered;
	for (auto TriggerTarget : TriggerTargets)
	{
		IReactToTriggerInterface* ReactTarget = Cast<IReactToTriggerInterface>(TriggerTarget);
		if (ReactTarget)
		{
			ReactTarget->Execute_ReactToTrigger(TriggerTarget);
		}
	}
}
