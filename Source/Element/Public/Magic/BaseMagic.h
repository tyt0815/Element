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
	FORCEINLINE FVector GetHitBoxTraceBackLocation();
	FORCEINLINE FVector GetHitBoxTraceFrontLocation();
	FORCEINLINE FVector GetHitBoxTraceLeftLocation();
	FORCEINLINE FVector GetHitBoxTraceRightLocation();
	FORCEINLINE FVector GetHitBoxTraceDownLocation();
	FORCEINLINE FVector GetHitBoxTraceUpLocation();
	FORCEINLINE FVector GetHitBoxTraceBackToFrontHalfSize();
	FORCEINLINE FVector GetHitBoxTraceLeftToRightHalfSize();
	FORCEINLINE FVector GetHitBoxTraceDownToUpHalfSize();
	void AddActorsToIgnore(AActor* Actor);
	void RemoveActorsToIgnore(AActor* Actor);
	void BoxTrace(FHitResult& HitResult);
	void BoxTrace(FHitResult& HitResult, TArray<AActor*>& Ignore);
	void BoxTraceMulti(TArray<FHitResult>& HitResults);
	void BoxTraceMulti(TArray<FHitResult>& HitResults, TArray<AActor*>& Ignore);
	void DamageActor(FHitResult& HitResult, float Damage, EFourElement Element);
	void DamageActorMulti(TArray<FHitResult>& HitResults, float Damage, EFourElement Element);
	virtual void SetMultiStageHit(float Damage, float Delay, EFourElement Element);
	void PushLiftableActor(AActor* Actor, FVector Force);

	virtual void MultiStageHit_v2(float Damage, EFourElement Element, float Delay);

	UFUNCTION()
	void RemoveFromActorsToIgnore(TArray<FHitResult> HitResults);


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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere);
	UBoxComponent* HitBoxComponent;

	UPROPERTY(VisibleAnywhere);
	UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float DamageCoefficient = 1;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float RepulsiveForce = 1000.0f;

	TArray<AActor*> ActorsToIgnore;
	FVector SpawnedLocation;
	TArray<TEnumAsByte<EObjectTypeQuery>> HitTraceObjectTypes;
	FTimerHandle MultiStageHitTimer;

	ABaseCharacter* Owner;

};