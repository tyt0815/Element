#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BasePlayerAnimInstance.generated.h"

class ABasePlayer;
class UCharacterMovementComponent;

UCLASS()
class ELEMENT_API UBasePlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	/*
	* Movement
	*/
protected:
	UPROPERTY(BlueprintReadOnly, Category = Movement);
	ABasePlayer* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement);
	UCharacterMovementComponent* PlayerMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement);
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = Movement);
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement);
	bool ShouldMove;

	UPROPERTY(BlueprintReadOnly, Category = Movement);
	bool IsFalling;

private:
	bool IsMove();
};
