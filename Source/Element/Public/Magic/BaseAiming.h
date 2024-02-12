#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseAiming.generated.h"

UCLASS()
class ELEMENT_API ABaseAiming : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseAiming();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetActorHiddenInGame(bool bNewHidden) override;

protected:
	void SetOnlyOwnerSee(UMeshComponent* MeshComponent);

private:

	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* Arrow;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* AimingMesh;
};
