#include "Magic/MagicBulletProjectile.h"
#include "Components/BoxComponent.h"

#include "Element/DebugMacro.h"

void AMagicBulletProjectile::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBoxOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	FHitResult HitResult;
	BoxTrace(HitResult);
	DamageActor(HitResult);
	Destroy();
}
