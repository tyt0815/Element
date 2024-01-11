#include "Magic/MagicCircle.h"
#include "NiagaraComponent.h"

#include "Magic/BaseMagic.h"
#include "Element/DebugMacro.h"

AMagicCircle::AMagicCircle()
{
	PrimaryActorTick.bCanEverTick = false;
	MagicCircle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MagicCircle"));
	SetRootComponent(MagicCircle);
}

void AMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	MagicCircle->OnSystemFinished.AddDynamic(this, &AMagicCircle::Deactivate);
}


void AMagicCircle::Activate_Implementation(FVector Location, FRotator Rotator)
{
	if (MagicCircle != nullptr)
	{
		MagicCircle->Activate();
		this->SetActorLocation(Location);
		this->SetActorRotation(Rotator);
	}
}

void AMagicCircle::Deactivate_Implementation(UNiagaraComponent* NiagaraComponent)
{
	Destroy();
}