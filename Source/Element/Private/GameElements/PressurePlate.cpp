#include "GameElements/PressurePlate.h"
#include "Components/BoxComponent.h"

#include "Interfaces/ReactToTriggerInterface.h"

APressurePlate::APressurePlate()
{
	PrimaryActorTick.bCanEverTick = false;

	ButtonFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonFrameMesh"));
	SetRootComponent(ButtonFrameMesh);
	ButtonFrameMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonMesh->SetupAttachment(GetRootComponent());
	ButtonMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
	PressureOverlap = CreateDefaultSubobject<UBoxComponent>(TEXT("PressureOverlap"));
	PressureOverlap->SetupAttachment(GetRootComponent());
	PressureOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PressureOverlap->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	PressureOverlap->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	PressureOverlap->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	PressureOverlap->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	PressureOverlap->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
	PressureOverlap->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	PressureOverlap->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	PressureOverlap->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
}

void APressurePlate::BeginPlay()
{
	Super::BeginPlay();
	
	PressureOverlap->OnComponentBeginOverlap.AddDynamic(this, &APressurePlate::BeginPressureOverlap);
	PressureOverlap->OnComponentEndOverlap.AddDynamic(this, &APressurePlate::EndPressureOverlap);
}

void APressurePlate::Trigger_Implementation()
{
	Triggered = true;
	for (auto TriggerTarget : TriggerTargets)
	{
		IReactToTriggerInterface::Execute_ReactToTrigger(TriggerTarget);
	}
}

void APressurePlate::Halt_Implementation()
{
	Triggered = false;
}

void APressurePlate::BeginPressureOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TArray<AActor*> OverlappingActors;
	PressureOverlap->GetOverlappingActors(OverlappingActors);
	if (!ITriggerInterface::Execute_IsTriggered(this) && OverlappingActors.Num() > 0)
	{
		ITriggerInterface::Execute_Trigger(this);
	}
}

void APressurePlate::EndPressureOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TArray<AActor*> OverlappingActors;
	PressureOverlap->GetOverlappingActors(OverlappingActors);
	if (ITriggerInterface::Execute_IsTriggered(this) && OverlappingActors.Num() == 0)
	{
		ITriggerInterface::Execute_Halt(this);
	}
}

