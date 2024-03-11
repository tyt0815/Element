#include "Magic/Piercing.h"

#include "Element/DebugMacro.h"

void APiercing::Tick(float DeltaTime)
{
	MultiStageHit_v2(GetOwnerATK() * DamageCoefficient, EFourElement::EPE_Ventus, PiercingDelay);
	Super::Tick(DeltaTime);
}

void APiercing::BeginPlay()
{
	Super::BeginPlay();
}
