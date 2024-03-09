#include "Magic/MagicBulletProjectile.h"
#include "Components/BoxComponent.h"

#include "Element/DebugMacro.h"

void AMagicBulletProjectile::BeginBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Magic"))) return;
	Super::BeginBoxOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	FHitResult HitResult;
	BoxTrace(HitResult);
	DamageActor(HitResult, GetOwnerATK() * DamageCoefficient, EFourElement::EPE_None);
	Destroy();
}
