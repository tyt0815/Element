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


void AMagicCircle::Activate_Implementation(FVector Location, FRotator Rotator, float Range)
{
	if (MagicCircle != nullptr)
	{
		MagicCircle->Activate();
		this->SetActorLocation(Location);
		this->SetActorRotation(Rotator);
	}

	SpawnMagicActor(Location, Rotator, Range);
}

void AMagicCircle::Deactivate_Implementation(UNiagaraComponent* NiagaraComponent)
{
	Destroy();
}

void AMagicCircle::SpawnMagicActor(UPARAM(ref)FVector& Location, UPARAM(ref)FRotator& Rotator, float Range)
{
	UWorld* World = GetWorld();
	if (World != nullptr && MagicClass != nullptr)
	{
		ABaseMagic* Magic = World->SpawnActor<ABaseMagic>(MagicClass, Location, Rotator);
		if (Magic != nullptr && GetOwner() != nullptr)
		{
			Magic->SetOwner(GetOwner());
			Magic->Activate(Location, Rotator, Range);
		}
	}
}
