#include "Magic/FlameStrike.h"

void AFlameStrike::BeginPlay()
{
	Super::BeginPlay();

	EndMagicAfter(0.5f);
}

void AFlameStrike::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBoxOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	FHitResult HitResult;
	BoxTrace(HitResult);
	DamageActor(HitResult);
}
