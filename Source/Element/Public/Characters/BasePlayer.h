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
class USphereComponent;
class ABaseMagic;
class ABaseAiming;
class IInteractionInterface;

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
	* Inputs
	*/
private:
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
	void InteractionTriggered(const FInputActionInstance& Instance);

	UPROPERTY(EditAnywhere, Category = "Attributes | Input | InputMappingContext");
	UInputMappingContext* KBMMappingContext = nullptr;

	UPROPERTY(EditAnywhere, Category = "Attributes | Input | InputAction");
	UInputAction* MoveAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Attributes | Input | InputAction");
	UInputAction* LookAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Attributes | Input | InputAction");
	UInputAction* JumpAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Attributes | Input | InputAction");
	UInputAction* InteractionAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Attributes | Input | InputAction");
	UInputAction* AttackAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Attributes | Input | InputAction");
	UInputAction* CastAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Attributes | Input | InputAction");
	UInputAction* ElementSelectAction1 = nullptr;

	UPROPERTY(EditAnywhere, Category = "Attributes | Input | InputAction");
	UInputAction* ElementSelectAction2 = nullptr;

	UPROPERTY(EditAnywhere, Category = "Attributes | Input | InputAction");
	UInputAction* ElementSelectAction3 = nullptr;

	UPROPERTY(EditAnywhere, Category = "Attributes | Input | InputAction");
	UInputAction* ElementSelectAction4 = nullptr;

	UPROPERTY(EditAnywhere, Category = "Attributes | Input | InputAction");
	UInputAction* MouseWheelAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Attributes | Input | InputAction");
	UInputAction* SubSkill1Action = nullptr;

	UPROPERTY(EditAnywhere, Category = "Attributes | Input | InputAction");
	UInputAction* SubSkill2Action = nullptr;

	/*
	* Camera
	*/
public:
	FORCEINLINE FVector GetCameraForwardVector() const;
	FORCEINLINE FRotator GetCameraRotation() const;
	FORCEINLINE FVector GetCameraRelativeLocation() const;
	FVector GetCameraLookAtLocation() const;

protected:
	void SwitchCameraLocation();
	void ZoomOutCamera();
	void ZoomInCamera();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly);
	USpringArmComponent* SpringArm;
private:
	UPROPERTY(EditAnywhere, Category = "Attributes | ViewCamera");
	float LookAtOffset = 5000.0f;

	UPROPERTY(EditAnywhere, Category = "Attributes | ViewCamera");
	float ZoomSpringArmLength = 250.0f;

	UPROPERTY(EditAnywhere, Category = "Attributes | ViewCamera");
	float ZoomCameraOffset = 40.0f;

	UPROPERTY(EditAnywhere, Category = "Attributes | ViewCamera");
	float CameraMoveRate = 0.1;

	EPlayerCameraState CameraState = EPlayerCameraState::EPCS_ZoomOut;
	float OriginSpringArmLength;
	FVector OriginCameraLocation;
	FVector TargetCameraLocation;

	/*
	* HUDs
	*/
protected:
	void ShowInteractionWidget(bool WidgetShowed, AActor* InteractiveActor);
	void UpdateElementSlotUI();

private:
	void InitPlayerOverlay();

	UPROPERTY()
	UPlayerOverlay* PlayerOverlay;

	/*
	* Actions
	*/
public:
	FORCEINLINE void SetPlayerActionState(EPlayerActionState NewState) { PlayerActionState = NewState; }
	FORCEINLINE void AddInteractionTarget(AActor* InteractiveActor) { InteractionTargets.Add(InteractiveActor); }
	FORCEINLINE void RemoveInteractionTarget(AActor* InteractiveActor) { InteractionTargets.Remove(InteractiveActor); }
	FORCEINLINE bool IsInteractiveActor(AActor* Actor) const;
	void SetTargetInteractiveActor();

private:
	void Move(const FInputActionInstance& Instance);
	void Look(const FInputActionInstance& Instance);

	UPROPERTY(EditAnywhere, Category = "Attributes | Actions");
	float WalkSpeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Attributes | Actions");
	float InteractionRange = 700.0f;

	UPROPERTY(EditAnywhere, Category = "Attributes | Actions");
	float InteractionTraceInnerRadius = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Attributes | Actions");
	float InteractionTraceOutterRadius = 100.0f;

	float RunSpeed;
	EPlayerActionState PlayerActionState = EPlayerActionState::EPAS_Unoccupied;
	TSet<AActor*> InteractionTargets;
	AActor* TargetInteractiveActor;

	/*
	* Combats
	*/
public:
	void InitMagicCircleDistVariationSpeed();
	void InitElementsArray(EFourElement First, EFourElement Second, EFourElement Third, EFourElement Forth);
	void InitElementsReadyArray(EFourElement First, EFourElement Second, EFourElement Third, EFourElement Forth);
	FVector GetChestLocation();
	FVector GetCharacterFrontMagicCircle();
	FRotator GetCharacterFrontMagicCircleRotator();
	FRotator GetFlyMagicCircleRotator();
	EFourElement GetSelectedElement(uint8 i);
	void SetCurrMagicCircleDist(float Value);
	void SetSelectedElement(uint8 i, EFourElement Element);
	bool IsElementSeleted();
	bool IsBlocked(FVector Start, FVector End, FHitResult& BlockingActor, TArray<AActor*> ActorsToIgnore = TArray<AActor*>());
	bool IsCoolDown(FTimerHandle& CoolTimer);
	void IncreaseMagicCircleDistVariationSpeed();
	void EmptyElementsSeletedArray();
	void SelectElement(uint8 Index);
	void UseSelectedElements();

protected:
	bool LocateCharacterFrontMagicCircle(FVector Offset, FVector& Location);
	bool LocateFloorMagicCircle(FVector Offset, FVector& Location);
	bool LocateFlyMagicCircle(FVector Offset, FVector& Location);
	ABaseAiming* SpawnAimingActor(TSubclassOf<ABaseAiming> AimingClass);
	UNiagaraComponent* SpawnMagicCircle(FVector Location, FRotator Rotator, UNiagaraSystem* MagicCircle);
	ABaseMagic* SpawnMagicActor(FVector Location, FRotator Rotator, TSubclassOf<ABaseMagic> MagicClass);
	void ShowFloorAimingCircle();
	void ShowFlyAimingCircle();
	ABaseMagic* UseFloorMagic(UNiagaraSystem* MagicCircle, TSubclassOf<ABaseMagic> MagicClass);
	void CastEnd();
	void MagicII_FlameStrike();
	void MagicAA_HealOverTime();
	void MagicVV_Piercing();
	void MagicTT_Portal();
	void MagicIV_Explosion();
	void MagicVA_Tornado();
	void MagicAT_Summon();
	void MagicTI_Meteorite();

private:
	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | MagicCircle");
	TSubclassOf<ABaseAiming> FloorAimingClass;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | MagicCircle");
	TSubclassOf<ABaseAiming> FlyAimingClass;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | MagicCircle");
	float ChestLocationZOffset = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | MagicCircle");
	float CharacterFrontCastableRange = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | MagicCircle");
	float FlyMagicCircleCastableRange = 150.0f;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | MagicCircle");
	FVector FloorMagicCircleBoxTraceHalf = FVector(0.0f, 0.0f, 50.0f);

	/*Fly, Floor MagicCircle*/
	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | MagicCircle");
	float MagicCircleRange = 2000.0f; 

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Magic Bullet");
	UNiagaraSystem* MagicBulletCircle;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Magic Bullet");
	TSubclassOf<ABaseMagic> MagicBulletClass;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Magic Bullet");
	float MagicBulletCoolTime = 0.2;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Magic Bullet");
	FVector MagicBulletLocationOffset = FVector(20.0f, 40.0f, 40.0f);

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Flame Strike");
	UNiagaraSystem* FlameStrikeCircle;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Flame Strike");
	TSubclassOf<ABaseMagic> FlameStrikeClass;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Heal Over Time");
	UNiagaraSystem* HealOverTimeCircle;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Heal Over Time");
	float AmountOfHealPerTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Heal Over Time");
	int HealCount = 10;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Heal Over Time");
	float HealDelay = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Piercing");
	UNiagaraSystem* PiercingCircle;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Piercing");
	TSubclassOf<ABaseMagic> PiercingClass;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Portal");
	TSubclassOf<ABaseMagic> PortalClass;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Explosion");
	UNiagaraSystem* ExplosionCircle;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Explosion");
	TSubclassOf<ABaseMagic> ExplosionClass;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Tornado");
	UNiagaraSystem* TornadoCircle;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Tornado");
	TSubclassOf<ABaseMagic> TornadoClass;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Summon");
	UNiagaraSystem* SummonCircle;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Summon");
	TSubclassOf<ABaseMagic> SummonClass;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Summon");
	FVector SummonLocationCenter = FVector(-100, 0, 0);

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Summon");
	float SummonLocationRadius = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Meteorite");
	UNiagaraSystem* MeteoriteCircle;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Meteorite");
	TSubclassOf<ABaseMagic> MeteoriteClass;

	UPROPERTY(EditAnywhere, Category = "Attributes | Magic | Meteorite");
	FVector MeteoriteSpawnOffset = { 0.0f, 0.0f, 1000.0f };

	ECastedMagic CastedMagic = ECastedMagic::ECM_None;
	TArray<EFourElement> ElementsArray;
	TArray<EFourElement> ElementsReadyArray;
	TArray<int8> ElementsSelectedArray;
	FTimerHandle MagicBulletTimer;
	FTimerHandle HealHandler;
	ABaseAiming* FloorAimingActor;
	ABaseAiming* FlyAimingActor;
	float CurrMagicCircleDist;
	float MagicCircleDistVariationSpeed;
};