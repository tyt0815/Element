#include "GameElements/BaseTrigger.h"

#include "Interfaces/ReactToTriggerInterface.h"


ABaseTrigger::ABaseTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABaseTrigger::BeginPlay()
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

void ABaseTrigger::Trigger_Implementation(AActor* TriggeringgActor)
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
