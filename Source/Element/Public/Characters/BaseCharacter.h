#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "CharacterTypes.h"
#include "BaseCharacter.generated.h"

class UAttributeComponent;

UCLASS()
class ELEMENT_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	//
	// 기타
	//
public:
	FORCEINLINE FVector GetUnderCharacterFeetLocation();
	float GetATK();
	
	/*
	* 전투 시스템
	*/
public:
	FORCEINLINE UAttributeComponent* GetAttributes() { return Attributes; }
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

protected:
	void HealCharacter(float Value);

	UFUNCTION()
	void HealOverTimeCharacter(float Value, int Count, float Delay);

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;
};