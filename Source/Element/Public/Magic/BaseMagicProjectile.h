#pragma once

#include "CoreMinimal.h"
#include "Magic/BaseMagic.h"
#include "BaseMagicProjectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class ELEMENT_API ABaseMagicProjectile : public ABaseMagic
{
	GENERATED_BODY()
public:
	ABaseMagicProjectile();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	/*
	* Projectile Attribute
	*/
public:
	virtual void Activate_Implementation(FVector Location, FRotator Rotator, float Range);
	virtual void Deactivate_Implementation();

private:
	UPROPERTY(VisibleAnywhere);
	UProjectileMovementComponent* ProjectileMovementComponent;

	FVector SpawnedLocation;
};
