#pragma once

#include "CoreMinimal.h"
#include "GameElements/BaseInteractiveActor.h"
#include "BaseLiftableActor.generated.h"

class ABasePlayer;

UCLASS()
class ELEMENT_API ABaseLiftableActor : public ABaseInteractiveActor
{
	GENERATED_BODY()
	
public:
	ABaseLiftableActor();
	virtual void Tick(float DeltaTime) override;

	void RotateToLiftedRotation(float DeltaTime);

	void MoveToLiftedLocation(float DeltaTime);

public:
	virtual void Interact_Implementation(AActor* InteractingActor) override;

	void LayDown();

	UFUNCTION(BlueprintPure, Category = Lift)
	FORCEINLINE bool IsLifted() { return Lifted; }

	UFUNCTION(BlueprintPure, Category = Lift)
	FORCEINLINE ABasePlayer* GetLiftingActor() { return LiftingActor; }

protected:
	virtual void EndOverlapInteractRangeComponent_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	UPROPERTY(BlueprintGetter=IsLifted, Category = Lift)
	bool Lifted = false;

	UPROPERTY(BlueprintGetter=GetLiftingActor, Category = Lift)
	ABasePlayer* LiftingActor;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float LiftingZOffset = 100.0f;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float LiftingDistance = .8f;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float MoveLerpAlpha = 10.0f;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float RotateLerpAlpha = 10.0f;
};
