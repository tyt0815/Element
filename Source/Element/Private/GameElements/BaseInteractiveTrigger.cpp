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

void ABaseInteractiveTrigger::Interact_Implementation()
{
	Execute_Trigger(this);
}

void ABaseInteractiveTrigger::Trigger_Implementation()
{
	DeactivateInteraction();
	for (auto TriggerTarget : TriggerTargets)
	{
		IReactToTriggerInterface* ReactTarget = Cast<IReactToTriggerInterface>(TriggerTarget);
		if (ReactTarget)
		{
			ReactTarget->Execute_ReactToTrigger(TriggerTarget);
		}
	}
}

void ABaseInteractiveTrigger::ActivateInteraction()
{
	Super::ActivateInteraction();
	Triggered = false;
}

void ABaseInteractiveTrigger::DeactivateInteraction()
{

	Super::DeactivateInteraction();
	Triggered = true;
}
