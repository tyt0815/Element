#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

class UNiagaraComponent;
class UBaseAbilityComponent;
class ABaseMagic;

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
	void Activate(FVector Location, FRotator Rotator);
	virtual void Activate_Implementation(FVector Location, FRotator Rotator);

	UFUNCTION(BlueprintNativeEvent, Category = "Magic Circle")
	void Deactivate(UNiagaraComponent* NiagaraComponent);
	virtual void Deactivate_Implementation(UNiagaraComponent* NiagaraComponent);

private:
	UPROPERTY(EditAnywhere, Category = MagicCircle);
	UNiagaraComponent* MagicCircle;
};
