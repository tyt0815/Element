#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "BasePlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UPlayerOverlay;
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
	* 기본 Movement, Input
	*/
private:	
	void Move(const FInputActionInstance& Instance);
	void Look(const FInputActionInstance& Instance);
	void AttackStarted(const FInputActionInstance& Instance);
	void AttackOngoing(const FInputActionInstance& Instance);
	void AttackTriggered(const FInputActionInstance& Instance);
	void CastStarted(const FInputActionInstance& Instance);
	void CastOngoing(const FInputActionInstance& Instance);
	void CastTriggered(const FInputActionInstance& Instance);
	void ElementSelectAction1Started(const FInputActionInstance& Instance);
	void ElementSelectAction2Started(const FInputActionInstance& Instance);
	void ElementSelectAction3Started(const FInputActionInstance& Instance);
	void ElementSelectAction4Started(const FInputActionInstance& Instance);

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

	UPROPERTY(EditAnywhere, Category = "Input | InputAction");
	UInputAction* ElementSelectAction1 = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input | InputAction");
	UInputAction* ElementSelectAction2 = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input | InputAction");
	UInputAction* ElementSelectAction3 = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input | InputAction");
	UInputAction* ElementSelectAction4 = nullptr;

	UPROPERTY(EditAnywhere, Category = "Locomotion");
	float WalkSpeed;
	
	UPROPERTY(EditAnywhere, Category = "Locomotion");
	float RunSpeed;

	/*
	* 카메라
	*/
public:
	FVector GetCameraLookAtLocation();

protected:
	void SwitchCameraLocation();
	void ZoomOutCamera();
	void ZoomInCamera();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "View");
	UCameraComponent* ViewCamera;

private:
	UPROPERTY(VisibleAnywhere, Category = "View");
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category = "View");
	float LookAtOffset = 5000.0f;

	UPROPERTY(EditAnywhere, Category = "View");
	float ZoomSpringArmLength;

	UPROPERTY(EditAnywhere, Category = "View");
	FVector ZoomCameraLocation;

	UPROPERTY(EditAnywhere, Category = "View");
	float CameraMoveRate = 0.1;

	float OriginSpringArmLength;
	FVector OriginCameraLocation;
	EPlayerCameraState CameraState = EPlayerCameraState::EPCS_ZoomOut;

	/*
	* 전투 시스템
	*/
public:
	FVector GetChestLocation();
	bool LocateCharacterFrontMagicCircle(FVector Offset, FVector& Location);
	bool LocateFloorMagicCircle(FVector Offset, FVector& Location);
	bool LocateFlyMagicCircle(FVector Offset, FVector& Location);
	bool IsBlocked(FVector Start, FVector End, FVector& BlockedLocation);
	bool IsCoolDown(FTimerHandle& CoolTimer);
	FRotator GetCharacterFrontMagicCircleRotator();
	FRotator GetFloorMagicCircleRotator();
	FRotator GetFlyMagicCircleRotator();
	void InitElementsArray(EFourElement First, EFourElement Second, EFourElement Third, EFourElement Forth);
	void InitElementsReadyArray(EFourElement First, EFourElement Second, EFourElement Third, EFourElement Forth);
	void EmptyElementsSeletedArray();
	void SelectElement(uint8 Index);
	void UseSelectedElements();

	void UpdateElementSlotUI();

	UFUNCTION(BlueprintCallable, Category = "Magic")
	void ActivateMagicCircle(FVector Location, FRotator Rotator, float Range, const TSubclassOf<AMagicCircle>& MagicCircleClass);

private:
	UPROPERTY(EditAnywhere, Category = "MagicCircle");
	float MagicCircleRange = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "MagicCircle | CharacterFrontMagicCircle");
	float ChestLocationZOffset = 70.0f;

	UPROPERTY(EditAnywhere, Category = "MagicCircle | CharacterFrontMagicCircle");
	float CharacterFrontCastableRange;

	UPROPERTY(EditAnywhere, Category = "MagicCircle | FlyMagicCircle");
	float FlyCastableRange;

	UPROPERTY(EditAnywhere, Category = "MagicCircle | FlyMagicCircle");
	FVector FlyMagicCircleBoxTraceHalf;

	UPROPERTY(EditAnywhere, Category = "Magic | Magic Bullet");
	float MagicBulletCoolTime;

	UPROPERTY(EditAnywhere, Category = "Magic | Magic Bullet");
	FVector MagicBulletLocationOffset;

	UPROPERTY(EditAnywhere, Category = "Magic | Magic Bullet");
	TSubclassOf<AMagicCircle> MagicBulletCircleClass;

	UPROPERTY(EditAnywhere, Category = "Magic | Magic Bullet");
	float MagicBulletRange = 2000.0f;

	

	
	EPlayerActionState PlayerActionState = EPlayerActionState::EPAS_Unoccupied;
	ECastedMagic CastedMagic = ECastedMagic::ECM_None;
	TArray<EFourElement> ElementsArray;
	TArray<EFourElement> ElementsReadyArray;
	TArray<int8> ElementsSelectedArray;
	FTimerHandle MagicBulletTimer;

	/*
	* HUD
	*/
private:
	void InitPlayerOverlay();

	UPROPERTY()
	UPlayerOverlay* PlayerOverlay;
};
