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

	bool IsOverRange();
	double GetMovingDistance();

protected:
	virtual void BeginPlay() override;

	/*
	* Projectile Attribute
	*/
public:
	FORCEINLINE void SetProjectileRange(float Range) { ProjectileRange = Range; }
protected:
	UPROPERTY(VisibleAnywhere);
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float ProjectileRange = 100000.0f;
private:
};
