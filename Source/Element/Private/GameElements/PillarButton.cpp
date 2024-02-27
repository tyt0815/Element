#include "GameElements/PillarButton.h"


APillarButton::APillarButton()
{
	PillarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PillarMesh"));
	PillarMesh->SetupAttachment(GetRootComponent());
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonMesh->SetupAttachment(PillarMesh);
}

void APillarButton::BeginPlay()
{
	Super::BeginPlay();
}
