#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "BaseCharacter.generated.h"

UCLASS()
class ELEMENT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	/*
	* Math
	*/
protected:
	FVector VectorClamp(const FVector& Vector, const FVector& MinVector, const FVector& MaxVector);
};
