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
	* Magic Basic
	*/
public:
	void SetDamage(float Value) { Damage = Value; }
	virtual void InitActorsToIgnore();
	virtual void InitBoxTraceObjectTypes();
protected:
	void AddActorsToIgnore(AActor* Actor);
	void RemoveActorsToIgnore(AActor* Actor);
	void BoxTrace(FHitResult& HitResult);
	void DamageActor(FHitResult& HitResult);
	void EndMagicAfter(float Time);

	UFUNCTION()
	void EndMagic();

	UFUNCTION()
	virtual void BeginBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void EndBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere);
	UBoxComponent* HitBoxComponent;

	UPROPERTY(VisibleAnywhere);
	UArrowComponent* ArrowComponent;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	FVector BoxTraceHalfSize;
	FRotator BoxTraceOrientation;
	TArray<AActor*> ActorsToIgnore;
	float Damage = 1;
	bool IsActivated = false;
	FVector SpawnedLocation;
	FTimerHandle DestroyTimer;
	TArray<TEnumAsByte<EObjectTypeQuery>> BoxTraceObjectTypes;

private:
};
