#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Element/DebugMacro.h"
#include "Characters/CharacterTypes.h"
#include "BaseMagic.generated.h"

class ABaseCharacter;
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
	FORCEINLINE UBoxComponent* GetHitBoxComponent() const { return HitBoxComponent; }
	virtual void InitActorsToIgnore();
	virtual void InitHitTraceObjectTypes();
protected:
	FORCEINLINE float GetOwnerATK();
	FORCEINLINE FVector GetOwnerLocation();
	FORCEINLINE FRotator GetOwnerRotation();
	void AddActorsToIgnore(AActor* Actor);
	void RemoveActorsToIgnore(AActor* Actor);
	void BoxTrace(FHitResult& HitResult);
	void BoxTrace(FHitResult& HitResult, TArray<AActor*>& Ignore);
	void DamageActor(FHitResult& HitResult, float Damage, EFourElement Element);
	virtual void SetMultiStageHit(float Damage, float Delay, EFourElement Element);
	void PushLiftableActor(AActor* Actor, FVector Force);


	UFUNCTION()
	virtual void MultiStageHit(float Damage, EFourElement Element);

	UFUNCTION()
	virtual void EndMagic();

	UFUNCTION()
	virtual void BeginBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}

	UFUNCTION()
	virtual void EndBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootSceneComponent;

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

	UPROPERTY(EditAnywhere, Category = Attribute)
	float RepulsiveForce = 1000.0f;

	FVector BoxTraceHalfSize;
	TArray<AActor*> ActorsToIgnore;
	FVector SpawnedLocation;
	TArray<TEnumAsByte<EObjectTypeQuery>> HitTraceObjectTypes;
	FTimerHandle MultiStageHitTimer;

	ABaseCharacter* Owner;
};