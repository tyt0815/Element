#pragma once

#include "CoreMinimal.h"
#include "GameElements/BaseInteractiveActor.h"
#include "Door.generated.h"



UCLASS()
class ELEMENT_API ADoor : public ABaseInteractiveActor
{
	GENERATED_BODY()
	
public:
	ADoor();

protected:
	
	UFUNCTION(BlueprintPure)
	float GetOpenDirection(AActor* OpeningActor) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* DoorInnerMesh;
};
