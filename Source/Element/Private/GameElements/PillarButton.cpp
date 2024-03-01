#include "GameElements/PillarButton.h"


APillarButton::APillarButton()
{
	PrimaryActorTick.bCanEverTick = false;

	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMesh"));
	SetRootComponent(RootMesh);
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonMesh->SetupAttachment(GetRootComponent());
	ButtonMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void APillarButton::BeginPlay()
{
	Super::BeginPlay();

	ITriggerInterface::Execute_InitTriggerInterface(this, TriggerTargets);
}

void APillarButton::Interact_Implementation(AActor* InteractingActor)
{
	ITriggerInterface::Execute_IsTriggered(this) ?
		ITriggerInterface::Execute_Trigger(this, false, InteractingActor, TriggerTargets) : ITriggerInterface::Execute_Trigger(this, true, InteractingActor, TriggerTargets);
}
