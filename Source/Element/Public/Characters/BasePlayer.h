#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "BasePlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class USceneComponent;
class UMagicCircleComponent;

struct FInputActionInstance;

UCLASS()
class ELEMENT_API ABasePlayer : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	ABasePlayer();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere);
	TSubclassOf<AActor> Test;

	UPROPERTY(EditAnywhere);
	UMagicCircleComponent* MagicCircleComponent;

protected:
	void Move(const FInputActionInstance& Instance);
	void Look(const FInputActionInstance& Instance);


	/*
	* Ű ����
	*/
	UPROPERTY(EditAnywhere, Category = Input);
	UInputMappingContext* KBMMappingContext = nullptr;

	UPROPERTY(EditAnywhere, Category = Input);
	UInputAction* MoveAction = nullptr;

	UPROPERTY(EditAnywhere, Category = Input);
	UInputAction* LookAction = nullptr;

	UPROPERTY(EditAnywhere, Category = Input);
	UInputAction* JumpAction = nullptr;

	/*
	* ī�޶� ����
	*/
	UPROPERTY(VisibleAnywhere);
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere);
	UCameraComponent* ViewCamera;

	/*
	* ���� �ý���
	*/

private:
};
