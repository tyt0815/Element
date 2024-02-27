#include "Magic/FlameStrike.h"

void AFlameStrike::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(0.5f);
}

void AFlameStrike::BeginBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FHitResult HitResult;
	BoxTrace(HitResult);
	DamageActor(HitResult, GetOwnerATK() * DamageCoefficient);
}
