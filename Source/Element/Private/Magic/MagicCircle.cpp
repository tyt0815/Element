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
	MagicRange = Range;
}

void AMagicCircle::Deactivate_Implementation(UNiagaraComponent* NiagaraComponent)
{
	Destroy();
}

void AMagicCircle::SpawnMagicActor()
{
	UWorld* World = GetWorld();
	if (World != nullptr && MagicClass != nullptr)
	{
		ABaseMagic* Magic = World->SpawnActor<ABaseMagic>(MagicClass, GetActorLocation(), GetActorRotation());
		if (Magic != nullptr && GetOwner() != nullptr)
		{
			Magic->SetOwner(GetOwner());
			Magic->Activate(GetActorLocation(), GetActorRotation(), MagicRange);
		}
	}
}

void AMagicCircle::DelaySpawnMagicActor()
{
	if (Delay > 0)
	{
		GetWorldTimerManager().SetTimer(SpawnDelayTimer, this, &AMagicCircle::SpawnMagicActor, Delay);
	}
	else
	{
		SpawnMagicActor();
	}
}
