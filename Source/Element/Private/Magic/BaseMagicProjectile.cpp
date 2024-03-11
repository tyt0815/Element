#include "Magic/BaseMagicProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Element/DebugMacro.h"
#include "Characters/BasePlayer.h"
#include "GameElements/BaseLiftableActor.h"

ABaseMagicProjectile::ABaseMagicProjectile(): ABaseMagic()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	ProjectileMovementComponent->InitialSpeed = 5000.0f;
	ProjectileMovementComponent->MaxSpeed = 5000.0f;
}

void ABaseMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MovingDistance += GetMovingDistancePerTick(DeltaTime);
	if (IsOverRange())
	{
		EndMagic();
	}
}

bool ABaseMagicProjectile::IsOverRange()
{
	return MovingDistance > ProjectileRange;
}

void ABaseMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
}

double ABaseMagicProjectile::GetMovingDistancePerTick(float DeltaTime)
{
	return ProjectileMovementComponent->GetMaxSpeed() * DeltaTime;
}

void ABaseMagicProjectile::BeginBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginBoxOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	PushLiftableActor(OtherActor, GetActorForwardVector() * PushForce);
}
