#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

class UNiagaraComponent;
class UBaseAbilityComponent;
class ABaseMagic;
class UPrimitiveComponent;

UCLASS()
class ELEMENT_API AMagicCircle : public AActor
{
	GENERATED_BODY()
	
public:	
	AMagicCircle();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	/*
	* Basic MagicCircle Function
	*/
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Magic Circle")
	void Activate(FVector Location, FRotator Rotator, float Range);
	virtual void Activate_Implementation(FVector Location, FRotator Rotator, float Range);

	UFUNCTION(BlueprintNativeEvent, Category = "Magic Circle")
	void Deactivate(UNiagaraComponent* PSystem);
	virtual void Deactivate_Implementation(UNiagaraComponent* PSystem);

	UFUNCTION(BlueprintCallable, Category = "Magic Circle")
	void SpawnMagicActor();

	UFUNCTION(BlueprintCallable, Category = "Magic Circle")
	void DelaySpawnMagicActor();

protected:
	UPROPERTY(EditAnywhere, Category = "Magic Bullet Circle");
	float Delay = 0;

	FTimerHandle SpawnDelayTimer;

	float MagicRange;

private:

	UPROPERTY(EditAnywhere, Category = MagicCircle);
	UNiagaraComponent* MagicCircle;

	UPROPERTY(EditAnywhere, Category = MagicCircle);
	TSubclassOf<ABaseMagic> MagicClass;
};
