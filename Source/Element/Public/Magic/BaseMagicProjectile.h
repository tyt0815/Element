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
	FORCEINLINE void SetProjectileRange(float Range) { ProjectileRange = Range; }
protected:

private:
	UPROPERTY(VisibleAnywhere);
	UProjectileMovementComponent* ProjectileMovementComponent;

	
	float ProjectileRange = 100000.0f;
};
