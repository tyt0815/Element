#pragma once

#include "CoreMinimal.h"
#include "Magic/BaseMagic.h"
#include "Portal.generated.h"


UCLASS()
class ELEMENT_API APortal : public ABaseMagic
{
	GENERATED_BODY()
public:
	APortal();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE void SetOutPortal(APortal* Portal);
	FORCEINLINE void SetPortalLifeTime(float Value);
protected:
	virtual void BeginBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void EndBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	virtual void InitActorsToIgnore() override;
	virtual void InitBoxTraceObjectTypes() override;

	APortal* OutPortal;
	float PortalLifeTime;
};
