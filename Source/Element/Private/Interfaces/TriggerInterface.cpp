#include "Interfaces/TriggerInterface.h"

#include "Interfaces/ReactToTriggerInterface.h"

//ITriggerInterface::ITriggerInterface()
//{
//	SetTriggerTargets();
//	InitTriggerTargets();
//}
//
//void ITriggerInterface::Trigger_Implementation(AActor* TriggeringActor)
//{
//	Triggered = true;
//	SignalToTargets();
//}
//
//void ITriggerInterface::Halt_Implementation(AActor* HaltingActor)
//{
//	Triggered = false;
//	SignalToTargets();
//}
//
//void ITriggerInterface::SignalToTargets()
//{
//	if (TriggerTargets.Num() == 0) InitTriggerTargets();
//	for (auto TriggerTarget : TriggerTargets)
//	{
//		IReactToTriggerInterface* ReactTarget = Cast<IReactToTriggerInterface>(TriggerTarget);
//		if (ReactTarget)
//		{
//			ReactTarget->Execute_ReactToTrigger(TriggerTarget);
//		}
//	}
//}
//
//void ITriggerInterface::InitTriggerTargets()
//{
//	for (auto TriggerTarget : TriggerTargets)
//	{
//		IReactToTriggerInterface* ReactTarget = Cast<IReactToTriggerInterface>(TriggerTarget);
//		if (ReactTarget)
//		{
//			//ReactTarget->Execute_AddTrigger(TriggerTarget, this);
//		}
//	}
//}
