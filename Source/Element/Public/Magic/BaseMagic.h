#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseMagic.generated.h"

class UBoxComponent;
class UArrowComponent;

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
	* Basic Components
	*/
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Magic")
	void Activate(FVector Location, FRotator Rotator, float Range);
	virtual void Activate_Implementation(FVector Location, FRotator Rotator, float Range);

	UFUNCTION(BlueprintNativeEvent, Category = "Magic")
	void Deactivate();
	virtual void Deactivate_Implementation();

	float MagicRange;

private:
	UPROPERTY(VisibleAnywhere, Category = Magic);
	UBoxComponent* HitBoxComponent;

	UPROPERTY(VisibleAnywhere, Category = Magic);
	UArrowComponent* ArrowComponent;
};
