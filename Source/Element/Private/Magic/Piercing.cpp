#include "Magic/Piercing.h"

#include "Element/DebugMacro.h"

void APiercing::BeginBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginBoxOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (!GetWorldTimerManager().IsTimerActive(PiercingTimer))
	{
		PiercingDamage();
	}	
}

void APiercing::PiercingDamage()
{
	
	FHitResult HitResult;
	InitActorsToIgnore();
	BoxTrace(HitResult);
	if (HitResult.GetActor())
	{
		DamageActor(HitResult);
		GetWorldTimerManager().SetTimer(PiercingTimer, this, &APiercing::PiercingDamage, PiercingDelay);
	}
}
