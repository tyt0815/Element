#include "GameElements/PortalSwitcher.h"

#include "Interfaces/TriggerInterface.h"
#include "Magic/Portal.h"


APortalSwitcher::APortalSwitcher()
{
	PrimaryActorTick.bCanEverTick = false;

}

void APortalSwitcher::ReactToTrigger_Implementation(AActor* Trigger)
{
	if (Switchers.Num() != SwitchedPortals.Num())
	{
		SCREEN_LOG_NONE_KEY(TEXT("APortalSwitcher: Switchers and SwitchedPortals Num is not equal"));
		return;
	}
	for (int i = 0; i < Switchers.Num() && i < SwitchedPortals.Num(); ++i)
	{
		if (!Switchers[i] || !SwitchedPortals[i])
		{
			Switchers.RemoveAt(i);
			SwitchedPortals.RemoveAt(i);
			--i;
			continue;
		}
		if (Trigger == Switchers[i] && ITriggerInterface::Execute_IsTriggered(Switchers[i]))
		{
			if (SwitchedPortals[i])
			{
				SwitchedPortals[i]->SetActorLocation(GetActorLocation());
				SwitchedPortals[i]->SetActorRotation(GetActorRotation());
				SwitchedPortals[i]->SetSceneCaptureRenderTarget(Portal1FrontRT, Portal1BackRT);
				if (SwitchedPortals[i]->GetOutPortal()) SwitchedPortals[i]->GetOutPortal()->SetSceneCaptureRenderTarget(Portal2FrontRT, Portal2BackRT);
			}
		}
		else
		{
			if (SwitchedPortals[i])
			{
				SwitchedPortals[i]->SetSceneCaptureRenderTarget(nullptr, nullptr);
				if (SwitchedPortals[i]->GetOutPortal())
				{
					SwitchedPortals[i]->SetActorLocation(SwitchedPortals[i]->GetOutPortal()->GetActorLocation());
					SwitchedPortals[i]->GetOutPortal()->SetSceneCaptureRenderTarget(nullptr, nullptr);
				}
				else SwitchedPortals[i]->SetActorLocation(FVector::ZAxisVector * 10000.0f);
			}

			if (Switchers[i]->Implements<UTriggerInterface>())
			{
				TArray<AActor*> TriggerTargets;
				ITriggerInterface::Execute_Trigger(Switchers[i], false, this, TriggerTargets);
			}
			
		}
	}
}

