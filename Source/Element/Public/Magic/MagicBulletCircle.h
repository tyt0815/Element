#pragma once

#include "CoreMinimal.h"
#include "Magic/MagicCircle.h"
#include "MagicBulletCircle.generated.h"

UCLASS()
class ELEMENT_API AMagicBulletCircle : public AMagicCircle
{
	GENERATED_BODY()
	
public:
	virtual void Activate_Implementation(FVector Location, FRotator Rotator, float Range) override;

	
};
