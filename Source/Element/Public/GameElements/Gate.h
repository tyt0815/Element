// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ReactToTriggerInterface.h"
#include "Element/DebugMacro.h"
#include "Gate.generated.h"

UCLASS()
class ELEMENT_API AGate : public AActor, public IReactToTriggerInterface
{
	GENERATED_BODY()
	
public:	
	AGate();

protected:
	virtual void BeginPlay() override;

public:
	virtual void ReactToTrigger_Implementation() override;
	

protected:

	UFUNCTION(BlueprintNativeEvent)
	void OpenGate();
	virtual void OpenGate_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void CloseGate();
	virtual void CloseGate_Implementation();

	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* RootArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* GateMesh;

private:
	ECollisionEnabled::Type OriginCollisionEnabled;
};
