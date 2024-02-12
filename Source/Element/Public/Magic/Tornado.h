// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Magic/BaseMagic.h"
#include "Tornado.generated.h"

/**
 * 
 */
UCLASS()
class ELEMENT_API ATornado : public ABaseMagic
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

protected:
	bool IsTopLocation(AActor* Actor);
	virtual void BeginBoxOverlapExec(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void EndBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float UpSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float TopSpeed = 4.1f;
	
	UPROPERTY(EditAnywhere, Category = Attribute)
	float TopOffset = 10.0f;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float UpForce = 1.0f;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float MSHDelay = 0.1f;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float LifeTime = 10.0f;

	FVector TopLocation;
};
