// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Magic/BaseAiming.h"
#include "BaseFlyAiming.generated.h"

class USplineMeshComponent;

UCLASS()
class ELEMENT_API ABaseFlyAiming : public ABaseAiming
{
	GENERATED_BODY()
public:
	ABaseFlyAiming();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetActorHiddenInGame(bool bNewHidden) override;

private:
	UPROPERTY(VisibleAnywhere)
	USplineMeshComponent* SplineMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* FloorMarkingMesh;

	TArray<TEnumAsByte<EObjectTypeQuery>> LineTraceObjectTypes;
};
