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

protected:
	void TeleportActor(AActor* Actor);
	virtual void EndBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	virtual void InitActorsToIgnore() override;
	virtual void InitBoxTraceObjectTypes() override;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float PortalLifeTime;

	APortal* OutPortal;
};
