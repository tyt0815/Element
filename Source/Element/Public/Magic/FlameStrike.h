#pragma once

#include "CoreMinimal.h"
#include "Magic/BaseMagic.h"
#include "FlameStrike.generated.h"


UCLASS()
class ELEMENT_API AFlameStrike : public ABaseMagic
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void BeginBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
