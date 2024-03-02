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

	/*
	* Character Attribute
	*/
public:
	FORCEINLINE UAttributeComponent* GetAttributes() { return Attributes; }
	float GetATK();

protected:
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	/*
	* Combat
	*/
public:
	FORCEINLINE FVector GetUnderCharacterFeetLocation();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	void HealCharacter(float Value);

	UFUNCTION()
	void HealOverTimeCharacter(float Value, int Count, float Delay);
};