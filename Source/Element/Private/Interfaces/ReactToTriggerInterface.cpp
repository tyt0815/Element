#include "Interfaces/ReactToTriggerInterface.h"

#include "Interfaces/TriggerInterface.h"

bool IReactToTriggerInterface::IsAllTriggersTriggered_Implementation()
{
	for (auto Trigger : Triggers)
	{
		ITriggerInterface* TriggerInterface = Cast<ITriggerInterface>(Trigger);
		if (!TriggerInterface->Execute_IsTriggered(Trigger)) return false;
	}
	return true;
}
