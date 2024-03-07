// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClearSpaceMarker.generated.h"

UCLASS()
class ELEMENT_API AClearSpaceMarker : public AActor
{
	GENERATED_BODY()
	
public:	
	AClearSpaceMarker();

protected:
	UFUNCTION(BlueprintCallable)
	void CheckClearSpace();
};
