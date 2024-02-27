#include "GameElements/BaseInteractiveActor.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

#include "Characters/BasePlayer.h"
#include "Interfaces/ReactToTriggerInterface.h"

ABaseInteractiveActor::ABaseInteractiveActor()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractRangeComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InteractRange"));
	SetRootComponent(InteractRangeComponent);
	InteractRangeComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractRangeComponent->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	InteractRangeComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InteractRangeComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));
	InteractWidget->SetupAttachment(GetRootComponent());
	InteractWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void ABaseInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	
	InteractRangeComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseInteractiveActor::BeginOverlapInteractRangeComponent);
	InteractRangeComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseInteractiveActor::EndOverlapInteractRangeComponent);
	InteractWidget->bHiddenInGame = true;
	ActivateInteraction();
}

void ABaseInteractiveActor::BeginOverlapInteractRangeComponent_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IInteractionInterface::Execute_IsInteractable(this)) return;
	TSet<AActor*> OverlappingActors;
	InteractRangeComponent->GetOverlappingActors(OverlappingActors);
	for (const auto OverlappingActor : OverlappingActors)
	{
		ABasePlayer* Player = Cast<ABasePlayer>(OverlappingActor);
		if (Player)
		{
			Player->AddInteractionTarget(this);
			InteractWidget->bHiddenInGame = false;
		}
	}
}

void ABaseInteractiveActor::EndOverlapInteractRangeComponent_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TSet<AActor*> OverlappingActors;
	InteractRangeComponent->GetOverlappingActors(OverlappingActors);
	if (OverlappingActors.Find(OtherActor) == nullptr)
	{
		ABasePlayer* Player = Cast<ABasePlayer>(OtherActor);
		if (Player)
		{
			Player->RemoveInteractionTarget(this);
			InteractWidget->bHiddenInGame = true;
		}
	}
}

void ABaseInteractiveActor::ActivateInteraction()
{
	Interactable = true;
	InteractRangeComponent->Activate();
}

void ABaseInteractiveActor::DeactivateInteraction()
{
	Interactable = false;
	InteractWidget->bHiddenInGame = true;
	InteractRangeComponent->Deactivate();
}

