#include "Magic/MagicBulletProjectile.h"
#include "Components/BoxComponent.h"

#include "Element/DebugMacro.h"

void AMagicBulletProjectile::BeginBoxOverlapExec(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginBoxOverlapExec(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	FHitResult HitResult;
	BoxTrace(HitResult);
	DamageActor(HitResult, GetOwnerATK() * DamageCoefficient);
	Destroy();
}
