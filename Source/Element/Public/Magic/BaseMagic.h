#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseMagic.generated.h"

class UBoxComponent;
class UArrowComponent;
class USceneComponent;
class UStaticMeshComponent;
class UNiagaraComponent;
class UAttributeComponent;

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
	virtual void InitActorsToIgnore();
	virtual void InitBoxTraceObjectTypes();
protected:
	FORCEINLINE float GetOwnerATK();
	void AddActorsToIgnore(AActor* Actor);
	void RemoveActorsToIgnore(AActor* Actor);
	void BoxTrace(FHitResult& HitResult);
	void DamageActor(FHitResult& HitResult, float Damage);
	void EndMagicAfter(float Time);
	virtual void BeginBoxOverlapExec(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void SetMultiStageHit(float Damage, float Delay);

	UFUNCTION()
	virtual void MultiStageHit(float Damage);

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

	UPROPERTY(EditAnywhere, Category = Attribute)
	float DamageCoefficient = 1;

	FVector BoxTraceHalfSize;
	FRotator BoxTraceOrientation;
	TArray<AActor*> ActorsToIgnore;
	FVector SpawnedLocation;
	TArray<TEnumAsByte<EObjectTypeQuery>> BoxTraceObjectTypes;
	FTimerHandle DestroyTimer;
	FTimerHandle MultiStageHitTimer;

	class ABaseCharacter* Owner;
};