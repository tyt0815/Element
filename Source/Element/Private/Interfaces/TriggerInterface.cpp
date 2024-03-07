#include "Interfaces/TriggerInterface.h"

#include "Interfaces/ReactToTriggerInterface.h"
#include "Element/DebugMacro.h"

void ITriggerInterface::InitTriggerInterface_Implementation(TArray<AActor*>& TriggerTargets)
{
	if (Initialized)
	{
		UObject* SelfAsUObject = Cast<UObject>(this);
		if (SelfAsUObject)
		{
			SCREEN_LOG_NONE_KEY(SelfAsUObject->GetName() + FString(": You already call ITriggerInterface::Execute_InitTriggerInterface"));
		}
		return;
	}
	Initialized = true;
	InitTriggerTargets(TriggerTargets);
}

void ITriggerInterface::Trigger_Implementation(bool Condition, AActor* TriggeringActor, TArray<AActor*>& TriggerTargets)
{
	Triggered = Condition;
	SignalToTargets(TriggerTargets);
}

void ITriggerInterface::SignalToTargets(TArray<AActor*>& TriggerTargets)
{
	AActor* SelfAsActor = Cast<AActor>(this);
	if (!SelfAsActor) return;
	if (!Initialized)
	{
		SCREEN_LOG_NONE_KEY(SelfAsActor->GetName() + FString(": You must call ITriggerInterface::Execute_InitTriggerInterface First"));
		return;
	}
	for (auto TriggerTarget : TriggerTargets)
	{
		IReactToTriggerInterface* ReactTarget = Cast<IReactToTriggerInterface>(TriggerTarget);

		if (ReactTarget)
		{
			ReactTarget->Execute_ReactToTrigger(TriggerTarget, SelfAsActor);
		}
	}
}

void ITriggerInterface::InitTriggerTargets(TArray<AActor*>& TriggerTargets)
{
	for (auto TriggerTarget : TriggerTargets)
	{
		if (TriggerTarget->Implements<UReactToTriggerInterface>())
		{
			IReactToTriggerInterface::Execute_AddTrigger(TriggerTarget, Cast<AActor>(this));
		}
		else
		{
			TriggerTargets.Remove(TriggerTarget);
		}
	}
}
