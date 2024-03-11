// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Interfaces/TriggerInterface.h"
#include "PillarButton.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENT_API APillarButton : public AActor, public IInteractionInterface, public ITriggerInterface
{
	GENERATED_BODY()
	
public:
	APillarButton();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void Interact_Implementation(AActor* InteractingActor) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* RootMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ButtonMesh;

	UPROPERTY(EditInstanceOnly, Category = Attributes)
	TArray<AActor*> TriggerTargets;
};
