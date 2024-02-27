#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/TriggerInterface.h"
#include "Element/DebugMacro.h"
#include "PressurePlate.generated.h"

class UBoxComponent;

UCLASS()
class ELEMENT_API APressurePlate : public AActor, public ITriggerInterface
{
	GENERATED_BODY()
	
public:	
	APressurePlate();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Trigger_Implementation() override;
	virtual void Halt_Implementation() override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ButtonFrameMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ButtonMesh;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* PressureOverlap;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BeginPressureOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void BeginPressureOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EndPressureOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	virtual void EndPressureOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	TArray<AActor*> TriggerTargets;
};
