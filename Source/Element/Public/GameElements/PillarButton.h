// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameElements/BaseInteractiveTrigger.h"
#include "PillarButton.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENT_API APillarButton : public ABaseInteractiveTrigger
{
	GENERATED_BODY()
	
public:
	APillarButton();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Trigger_Implementation(AActor* TriggeringgActor) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ButtonMesh;
};
