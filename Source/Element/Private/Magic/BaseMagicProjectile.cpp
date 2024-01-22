#include "Magic/BaseMagicProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Element/DebugMacro.h"
#include "Characters/BasePlayer.h"

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

	if ((SpawnedLocation - GetActorLocation()).Size() > MagicRange)
	{
		Deactivate_Implementation();
	}
}

void ABaseMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	SpawnedLocation = GetActorLocation();
}

void ABaseMagicProjectile::Activate_Implementation(FVector Location, FRotator Rotator, float Range)
{
	Super::Activate_Implementation(Location, Rotator, Range);
}

void ABaseMagicProjectile::Deactivate_Implementation()
{
	Super::Deactivate_Implementation();
}
