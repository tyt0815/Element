#include "Magic/Piercing.h"

#include "Element/DebugMacro.h"

void APiercing::BeginPlay()
{
	Super::BeginPlay();

	SetMultiStageHit(GetOwnerATK() * DamageCoefficient, PiercingDelay, EFourElement::EPE_Ventus);
}
