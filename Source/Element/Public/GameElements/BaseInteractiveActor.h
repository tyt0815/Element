#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Element/DebugMacro.h"
#include "BaseInteractiveActor.generated.h"

class USphereComponent;
class UWidgetComponent;

UCLASS()
class ELEMENT_API ABaseInteractiveActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ABaseInteractiveActor();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE float GetInteractRangeRadius();

protected:
	void BeTargetedToOverlappingActors();
	void BeUntargetedFromActor(AActor* OtherActor);
	bool IsOutOfTheRangeOfInteraction(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = Interaction)
	virtual void ActivateInteraction();

	UFUNCTION(BlueprintCallable, Category = Interaction)
	virtual void DeactivateInteraction();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interaction)
	void BeginOverlapInteractRangeComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void BeginOverlapInteractRangeComponent_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interaction)
	void EndOverlapInteractRangeComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	virtual void EndOverlapInteractRangeComponent_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* InteractRangeComponent;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* InteractWidget;
};
