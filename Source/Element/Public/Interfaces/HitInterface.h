#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Characters/CharacterTypes.h"
#include "HitInterface.generated.h"


UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ELEMENT_API IHitInterface
{
	GENERATED_BODY()

	
public:
	UFUNCTION(BlueprintNativeEvent)
	void GetHit(const FVector& ImpactPoint, AActor* Hitter, enum EFourElement Element);
};
