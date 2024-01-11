#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "BasePlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class AMagicCircle;

struct FInputActionInstance;

UCLASS()
class ELEMENT_API ABasePlayer : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	ABasePlayer();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	/*
	* 키
	*/
private:	
	void Move(const FInputActionInstance& Instance);
	void Look(const FInputActionInstance& Instance);
	void AttackOngoing(const FInputActionInstance& Instance);
	void CastOngoing(const FInputActionInstance& Instance);

	UPROPERTY(EditAnywhere, Category = "Input | InputMappingContext");
	UInputMappingContext* KBMMappingContext = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input | InputAction");
	UInputAction* MoveAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input | InputAction");
	UInputAction* LookAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input | InputAction");
	UInputAction* JumpAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input | InputAction");
	UInputAction* AttackAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input | InputAction");
	UInputAction* CastAction = nullptr;

	/*
	* 카메라
	*/
public:
	FVector GetCameraLookAtLocation();

private:

	UPROPERTY(VisibleAnywhere, Category = "Camera");
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera");
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere, Category = "Camera");
	float LookAtOffset = 3000.0f;

	/*
	* 전투 시스템
	*/
public:
	FVector GetFlyMagicCircleLocation(FVector& Offset);
	FRotator GetFlyMagicCircleRotator();
	FVector GetMagicCircleMiddlePointLocation();

	UFUNCTION(BlueprintCallable, Category = "Magic")
	void ActivateMagicCircle(FVector Location, FRotator Rotator, const TSubclassOf<AMagicCircle>& MagicCircleClass, TSubclassOf<ABaseMagic> MagicClass);

	UFUNCTION(BlueprintCallable, Category = "Magic")
	void SpawnMagicActor(UPARAM(ref) FVector& Location, UPARAM(ref) FRotator& Rotator, TSubclassOf<ABaseMagic> MagicClass);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Magic | Magic Bullet");
	float MagicBulletRange = 2000.0f;

private:
	UPROPERTY(EditAnywhere, Category = "Magic");
	FVector MagicCircleMiddlePointOffset = FVector(70.0f, 0.0f, 50.0f);

	UPROPERTY(EditAnywhere, Category = "Magic | Magic Bullet");
	float MagicBulletCoolTime;

	UPROPERTY(EditAnywhere, Category = "Magic | Magic Bullet");
	FVector MagicBulletLocationOffset;

	UPROPERTY(EditAnywhere, Category = "Magic | Magic Bullet");
	TSubclassOf<AMagicCircle> MagicBulletCircleClass;

	UPROPERTY(EditAnywhere, Category = "Magic | Magic Bullet");
	TSubclassOf<ABaseMagic> MagicBulletClass;

	FTimerHandle MagicBulletTimer;
};
