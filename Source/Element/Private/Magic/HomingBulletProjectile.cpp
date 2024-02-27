#include "Magic/HomingBulletProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AHomingBulletProjectile::AHomingBulletProjectile()
{
	ProjectileMovementComponent->bIsHomingProjectile = true;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
}

void AHomingBulletProjectile::SetHomingTarget(AActor* Actor)
{
	if (Actor)
	{
		ProjectileMovementComponent->HomingTargetComponent = Actor->GetRootComponent();
	}
}

void AHomingBulletProjectile::BeginBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Magic"))) return;
	FHitResult HitResult;
	BoxTrace(HitResult);
	DamageActor(HitResult, GetOwnerATK() * DamageCoefficient);
	EndMagic();
}