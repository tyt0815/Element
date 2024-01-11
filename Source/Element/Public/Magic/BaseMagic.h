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
private:
	UPROPERTY(VisibleAnywhere, Category = Magic);
	UBoxComponent* HitBoxComponent;

	UPROPERTY(VisibleAnywhere, Category = Magic);
	UArrowComponent* ArrowComponent;

public:
};
