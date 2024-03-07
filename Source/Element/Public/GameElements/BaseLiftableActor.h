#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "BaseLiftableActor.generated.h"

class ABasePlayer;

UCLASS()
class ELEMENT_API ABaseLiftableActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:
	ABaseLiftableActor();
	virtual void Tick(float DeltaTime) override;

public:
	UStaticMeshComponent* GetMesh() { return RootMesh; }
	virtual FString GetInteractionHint_Implementation() const override;
	virtual void Interact_Implementation(AActor* InteractingActor) override;
	void LayDown();

	UFUNCTION(BlueprintPure, Category = Random)
	FORCEINLINE int MakeRandomSeed();

	UFUNCTION(BlueprintPure, Category = Lift)
	FORCEINLINE bool IsLifted() { return Lifted; }

	UFUNCTION(BlueprintPure, Category = Lift)
	FORCEINLINE ABasePlayer* GetLiftingActor() { return LiftingActor; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* RootMesh;



private:
	void MoveToLiftedLocation(float DeltaTime);
	void RotateToLiftedRotation(float DeltaTime);

	UPROPERTY(BlueprintGetter=IsLifted, Category = Lift)
	bool Lifted = false;

	UPROPERTY(BlueprintGetter=GetLiftingActor, Category = Lift)
	ABasePlayer* LiftingActor;


	UPROPERTY(EditAnywhere, Category = Attribute)
	float LiftingDistance = 250.0f;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float MoveLerpAlpha = 10.0f;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float RotateLerpAlpha = 10.0f;

	FVector LiftingOffset;
};