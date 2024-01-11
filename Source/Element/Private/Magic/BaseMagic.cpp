#include "Magic/BaseMagic.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"

ABaseMagic::ABaseMagic()
{
	PrimaryActorTick.bCanEverTick = true;
	HitBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	SetRootComponent(HitBoxComponent);
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->SetupAttachment(GetRootComponent());
}

void ABaseMagic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseMagic::BeginPlay()
{
	Super::BeginPlay();
	HitBoxComponent->IgnoreActorWhenMoving(GetOwner(), true);
}