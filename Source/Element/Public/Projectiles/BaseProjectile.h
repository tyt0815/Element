#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UArrowComponent;

UCLASS()
class ELEMENT_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseProjectile();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	UBoxComponent* HitBoxComponent;

	UPROPERTY(VisibleAnywhere);
	UArrowComponent* ArrowComponent;

	UPROPERTY(VisibleAnywhere);
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere);
	UProjectileMovementComponent* ProjectileMovementComponent;
};
