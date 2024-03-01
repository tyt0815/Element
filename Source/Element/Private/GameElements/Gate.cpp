#include "GameElements/Gate.h"
#include "Components/ArrowComponent.h"

#include "Interfaces/TriggerInterface.h"

AGate::AGate()
{
	PrimaryActorTick.bCanEverTick = false;
	
	RootArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	SetRootComponent(RootArrow);
	GateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateMesh"));
	GateMesh->SetupAttachment(GetRootComponent());
	
}

void AGate::BeginPlay()
{
	Super::BeginPlay();
	OriginCollisionEnabled = GateMesh->GetCollisionEnabled();
}

void AGate::ReactToTrigger_Implementation(AActor* Trigger)
{
	IReactToTriggerInterface::Execute_IsAllTriggersTriggered(this) ? OpenGate() : CloseGate();
}

void AGate::OpenGate_Implementation()
{
	GateMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGate::CloseGate_Implementation()
{
	GateMesh->SetCollisionEnabled(OriginCollisionEnabled);
}
