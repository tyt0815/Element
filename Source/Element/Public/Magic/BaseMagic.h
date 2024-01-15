#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseMagic.generated.h"

class UBoxComponent;
class UArrowComponent;
class USceneComponent;
class UStaticMeshComponent;
class UNiagaraComponent;

UCLASS()
class ELEMENT_API ABaseMagic : public AActor
{
	GENERATED_BODY()
	
public:
	ABaseMagic();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	/*
	* Components Basic
	*/
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Magic")
	void Activate(FVector Location, FRotator Rotator, float Range);
	virtual void Activate_Implementation(FVector Location, FRotator Rotator, float Range);

	UFUNCTION(BlueprintNativeEvent, Category = "Magic")
	void Deactivate();
	virtual void Deactivate_Implementation();

	float MagicRange;

protected:
	void BoxTrace(TArray<FHitResult>& HitResults);
	
	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(VisibleAnywhere);
	UBoxComponent* HitBoxComponent;

	UPROPERTY(VisibleAnywhere);
	UArrowComponent* ArrowComponent;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category = "Magic | Trace")
	FVector BoxTraceHalfSize;

	UPROPERTY(EditAnywhere, Category = "Magic | Trace")
	FRotator BoxTraceOrientation;

	TArray<AActor*> ActorsToIgnore;
};
