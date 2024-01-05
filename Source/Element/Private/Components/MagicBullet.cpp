#include "Components/MagicBullet.h"
#include "Element/DebugMacro.h"

#include "Characters/BasePlayer.h"
#include "Components/MagicCircleComponent.h"

void UMagicBullet::OnStarted_Implementation(const FInputActionInstance& InputInstance)
{
	SCREEN_LOG_COLOR(0, FColor::Black, "OnStarted");
	ABasePlayer* Owner = Cast<ABasePlayer>(GetOwningCharacter());
	UWorld* World = GetWorld();
	if (Owner != nullptr && World)
	{
		World->SpawnActor<AActor>(Owner->Test, Owner->MagicCircleComponent->GetComponentTransform());
	}
}
