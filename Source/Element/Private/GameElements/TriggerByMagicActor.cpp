#include "GameElements/TriggerByMagicActor.h"
#include "Components/BoxComponent.h"

#include "Element/DebugMacro.h"

ATriggerByMagicActor::ATriggerByMagicActor()
{
	PrimaryActorTick.bCanEverTick = false;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);
	BaseMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(GetRootComponent());
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);
}

void ATriggerByMagicActor::BeginPlay()
{
	Super::BeginPlay();
	ITriggerInterface::Execute_InitTriggerInterface(this, Targets);
	
}

void ATriggerByMagicActor::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter, EFourElement Element)
{
	if (Element == TriggerElement)
	{
		ITriggerInterface::Execute_Trigger(this, true, Hitter, Targets);
	}
}

void ATriggerByMagicActor::Trigger_Implementation(bool Condition, AActor* TriggeringActor, TArray<AActor*>& TriggerTargets)
{
	ITriggerInterface::Trigger_Implementation(Condition, TriggeringActor, TriggerTargets);

	if (Condition && Duration > 0)
	{
		FTimerManager& WorldTimerManager = GetWorldTimerManager();
		WorldTimerManager.ClearTimer(FixedTermHandle);
		FTimerDelegate HaltTriggerDelegate;
		HaltTriggerDelegate.BindUFunction(this, FName("HaltTrigger"), TriggeringActor, TriggerTargets);
		WorldTimerManager.SetTimer(FixedTermHandle, HaltTriggerDelegate, Duration, false);
	}
	
}

void ATriggerByMagicActor::HaltTrigger(AActor* TriggeringActor, TArray<AActor*>& TriggerTargets)
{
	ITriggerInterface::Execute_Trigger(this, false, TriggeringActor, TriggerTargets);
}
