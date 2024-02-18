#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "BasePlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class UPlayerOverlay;
class ABaseMagic;
class ABaseAiming;

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
public:
	void InitMagicCircleDistVariationSpeed();
	void IncreaseMagicCircleDistVariationSpeed();
	void SetCurrMagicCircleDist(float Value);
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
	void MouseWheelTriggered(const FInputActionInstance& Instance);
	void SubSkill1Started(const FInputActionInstance& Instance);
	void SubSkill1Ongoing(const FInputActionInstance& Instance);
	void SubSkill1Triggered(const FInputActionInstance& Instance);
	void SubSkill2Started(const FInputActionInstance& Instance);
	void SubSkill2Ongoing(const FInputActionInstance& Instance);
	void SubSkill2Triggered(const FInputActionInstance& Instance);

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

	UPROPERTY(EditAnywhere, Category = "Input | InputAction");
	UInputAction* MouseWheelAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input | InputAction");
	UInputAction* SubSkill1Action = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input | InputAction");
	UInputAction* SubSkill2Action = nullptr;

	UPROPERTY(EditAnywhere, Category = "Locomotion");
	float WalkSpeed;
	
	UPROPERTY(EditAnywhere, Category = "Locomotion");
	float RunSpeed;

	float MagicCircleDistVariationSpeed;

	/*
	* 카메라
	*/
public:
	FVector GetCameraLookAtLocation();

protected:
	void SwitchCameraLocation();
	void ZoomOutCamera();
	void ZoomInCamera();
	void ShakeCamera(float Power);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	UCameraComponent* ViewCamera;

private:
	UPROPERTY(VisibleAnywhere);
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
	FVector TargetCameraLocation;
	EPlayerCameraState CameraState = EPlayerCameraState::EPCS_ZoomOut;

	/*
	* 전투 시스템
	*/
public:
	EFourElement GetSelectedElement(uint8 i);
	void SetSelectedElement(uint8 i, EFourElement Element);
	FVector GetChestLocation();
	FVector GetCharacterFrontMagicCircle();
	FRotator GetCharacterFrontMagicCircleRotator();
	FRotator GetFloorMagicCircleRotator();
	FRotator GetFlyMagicCircleRotator();
	bool LocateCharacterFrontMagicCircle(FVector Offset, FVector& Location);
	bool LocateFloorMagicCircle(FVector Offset, FVector& Location);
	bool LocateFlyMagicCircle(FVector Offset, FVector& Location);
	bool IsBlocked(FVector Start, FVector End, FVector& BlockedLocation, TArray<AActor*> ActorsToIgnore = TArray<AActor*>());
	bool IsCoolDown(FTimerHandle& CoolTimer);
	bool IsElementSeleted();
	void InitElementsArray(EFourElement First, EFourElement Second, EFourElement Third, EFourElement Forth);
	void InitElementsReadyArray(EFourElement First, EFourElement Second, EFourElement Third, EFourElement Forth);
	void EmptyElementsSeletedArray();
	void SelectElement(uint8 Index);
	void UseSelectedElements();
	void MagicII_FlameStrike();
	void MagicAA_HealOverTime();
	void MagicVV_Piercing();
	void MagicTT_Portal();
	void MagicIV_Explosion();
	void MagicVA_Tornado();
	void MagicAT_Summon();
	void MagicTI_Meteorite();
	void CastEnd();
	void UpdateElementSlotUI();
	ABaseMagic* UseFloorMagic(UNiagaraSystem* MagicCircle, TSubclassOf<ABaseMagic> MagicClass);
	ABaseAiming* SpawnAimingActor(TSubclassOf<ABaseAiming> AimingClass);
	UNiagaraComponent* SpawnMagicCircle(FVector Location, FRotator Rotator, UNiagaraSystem* MagicCircle);
	ABaseMagic* SpawnMagicActor(FVector Location, FRotator Rotator, TSubclassOf<ABaseMagic> MagicClass);
	void ShowFloorAimingCircle();
	void ShowFlyAimingCircle();

private:
	UPROPERTY(EditAnywhere, Category = "Magic | Magic Circle");
	TSubclassOf<ABaseAiming> FloorAimingClass;

	UPROPERTY(EditAnywhere, Category = "Magic | Magic Circle");
	TSubclassOf<ABaseAiming> FlyAimingClass;

	UPROPERTY(EditAnywhere, Category = "Magic | Magic Circle");
	float ChestLocationZOffset = 70.0f;

	UPROPERTY(EditAnywhere, Category = "Magic | Magic Circle");
	float CharacterFrontCastableRange;

	UPROPERTY(EditAnywhere, Category = "Magic | Magic Circle");
	float MagicCircleCastableRange;

	UPROPERTY(EditAnywhere, Category = "Magic | Magic Circle");
	float MagicCircleRange;

	UPROPERTY(EditAnywhere, Category = "Magic | Magic Circle");
	FVector FlyMagicCircleBoxTraceHalf;

	UPROPERTY(EditAnywhere, Category = "Magic | Magic Bullet");
	UNiagaraSystem* MagicBulletCircle;

	UPROPERTY(EditAnywhere, Category = "Magic | Magic Bullet");
	TSubclassOf<ABaseMagic> MagicBulletClass;

	UPROPERTY(EditAnywhere, Category = "Magic | Magic Bullet");
	float MagicBulletCoolTime;

	UPROPERTY(EditAnywhere, Category = "Magic | Magic Bullet");
	FVector MagicBulletLocationOffset;

	UPROPERTY(EditAnywhere, Category = "Magic | Flame Strike");
	UNiagaraSystem* FlameStrikeCircle;

	UPROPERTY(EditAnywhere, Category = "Magic | Flame Strike");
	TSubclassOf<ABaseMagic> FlameStrikeClass;

	UPROPERTY(EditAnywhere, Category = "Magic | Heal Over Time");
	UNiagaraSystem* HealOverTimeCircle;

	UPROPERTY(EditAnywhere, Category = "Magic | Heal Over Time");
	float AmountOfHealPerTime;

	UPROPERTY(EditAnywhere, Category = "Magic | Heal Over Time");
	int HealCount;

	UPROPERTY(EditAnywhere, Category = "Magic | Heal Over Time");
	float HealDelay;

	UPROPERTY(EditAnywhere, Category = "Magic | Piercing");
	UNiagaraSystem* PiercingCircle;

	UPROPERTY(EditAnywhere, Category = "Magic | Piercing");
	TSubclassOf<ABaseMagic> PiercingClass;

	UPROPERTY(EditAnywhere, Category = "Magic | Portal");
	TSubclassOf<ABaseMagic> PortalClass;

	UPROPERTY(EditAnywhere, Category = "Magic | Explosion");
	UNiagaraSystem* ExplosionCircle;

	UPROPERTY(EditAnywhere, Category = "Magic | Explosion");
	TSubclassOf<ABaseMagic> ExplosionClass;

	UPROPERTY(EditAnywhere, Category = "Magic | Explosion");
	float ExplosionBulletDamage = 1;

	UPROPERTY(EditAnywhere, Category = "Magic | Tornado");
	UNiagaraSystem* TornadoCircle;

	UPROPERTY(EditAnywhere, Category = "Magic | Tornado");
	TSubclassOf<ABaseMagic> TornadoClass;

	UPROPERTY(EditAnywhere, Category = "Magic | Summon");
	UNiagaraSystem* SummonCircle;

	UPROPERTY(EditAnywhere, Category = "Magic | Summon");
	TSubclassOf<ABaseMagic> SummonClass;

	UPROPERTY(EditAnywhere, Category = "Magic | Summon")
	FVector SummonLocationCenter = FVector(-100, 0, 0);

	UPROPERTY(EditAnywhere, Category = "Magic | Summon")
	float SummonLocationRadius = 30.0f;

	UPROPERTY(EditAnywhere, Category = "Magic | Meteorite");
	UNiagaraSystem* MeteoriteCircle;

	UPROPERTY(EditAnywhere, Category = "Magic | Meteorite");
	TSubclassOf<ABaseMagic> MeteoriteClass;

	UPROPERTY(EditAnywhere, Category = "Magic | Meteorite");
	FVector MeteoriteSpawnOffset = {0.0f, 0.0f, 1000.0f};

	EPlayerActionState PlayerActionState = EPlayerActionState::EPAS_Unoccupied;
	ECastedMagic CastedMagic = ECastedMagic::ECM_None;
	TArray<EFourElement> ElementsArray;
	TArray<EFourElement> ElementsReadyArray;
	TArray<int8> ElementsSelectedArray;
	FTimerHandle MagicBulletTimer;
	FTimerHandle HealHandler;
	ABaseAiming* FloorAimingActor;
	ABaseAiming* FlyAimingActor;
	float CurrMagicCircleDist;

	/*
	* HUD
	*/
private:
	void InitPlayerOverlay();

	UPROPERTY()
	UPlayerOverlay* PlayerOverlay;
};