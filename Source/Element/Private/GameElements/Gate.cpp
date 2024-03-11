#include "GameElements/Gate.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

#include "Interfaces/TriggerInterface.h"

AGate::AGate()
{
	PrimaryActorTick.bCanEverTick = false;
	
	RootArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	SetRootComponent(RootArrow);
	GateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateMesh"));
	GateMesh->SetupAttachment(GetRootComponent());
	GateMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(GateMesh);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

void AGate::BeginPlay()
{
	Super::BeginPlay();
	OriginCollisionEnabled = CollisionBox->GetCollisionEnabled();
}

void AGate::ReactToTrigger_Implementation(AActor* Trigger)
{
	bool Result;
	IReactToTriggerInterface::Execute_IsAllTriggersTriggered(this, Result);
	Result ? OpenGate() : CloseGate();
}

void AGate::OpenGate_Implementation()
{
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGate::CloseGate_Implementation()
{
	CollisionBox->SetCollisionEnabled(OriginCollisionEnabled);
}
