#include "GameElements/PillarButton.h"


APillarButton::APillarButton()
{
	BaseMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonMesh->SetupAttachment(GetRootComponent());
	ButtonMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void APillarButton::BeginPlay()
{
	Super::BeginPlay();
}

void APillarButton::Trigger_Implementation(AActor* TriggeringActor)
{
	Super::Trigger_Implementation(TriggeringActor);
}
