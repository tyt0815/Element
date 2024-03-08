#include "Interfaces/ReactToTriggerInterface.h"

#include "Interfaces/TriggerInterface.h"

//bool IReactToTriggerInterface::IsAllTriggersTriggered_Implementation()
//{
//	for (auto Trigger : Triggers)
//	{
//		ITriggerInterface* TriggerInterface = Cast<ITriggerInterface>(Trigger);
//		if (!TriggerInterface->Execute_IsTriggered(Trigger)) return false;
//	}
//	return true;
//}

void IReactToTriggerInterface::IsAllTriggersTriggered_Implementation(bool& Condition)
{
	Condition = true;
	for (auto Trigger : Triggers)
	{
		ITriggerInterface* TriggerInterface = Cast<ITriggerInterface>(Trigger);
		if (!TriggerInterface->Execute_IsTriggered(Trigger))
		{
			Condition = false;
			break;
		}
	}
}

void IReactToTriggerInterface::IsAllTriggersTriggerOff_Implementation(bool& Condition)
{
	Condition = true;
	for (auto Trigger : Triggers)
	{
		ITriggerInterface* TriggerInterface = Cast<ITriggerInterface>(Trigger);
		if (TriggerInterface->Execute_IsTriggered(Trigger))
		{
			Condition = false;
			break;
		}
	}
}
