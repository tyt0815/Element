#include "Magic/MagicBulletCircle.h"

#include "Element/DebugMacro.h"

void AMagicBulletCircle::Activate_Implementation(FVector Location, FRotator Rotator, float Range)
{
	Super::Activate_Implementation(Location, Rotator, Range);

	DelaySpawnMagicActor();
}
