#include "Characters/BasePlayer.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Element/DebugMacro.h"
#include "Magic/BaseAiming.h"
#include "Magic/BaseMagic.h"
#include "Magic/BaseMagicProjectile.h"
#include "Magic/Piercing.h"
#include "Magic/Portal.h"
#include "HUDs/PlayerHUD.h"
#include "HUDs/PlayerOverlay.h"

ABasePlayer::ABasePlayer() : ABaseCharacter()
{
	// 카메라 설정
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bUsePawnControlRotation = true;
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	InitElementsArray(EFourElement::EPE_Ignis, EFourElement::EPE_Aqua, EFourElement::EPE_Ventus, EFourElement::EPE_Terra);
	InitElementsReadyArray(EFourElement::EPE_Ignis, EFourElement::EPE_Aqua, EFourElement::EPE_Ventus, EFourElement::EPE_Terra);
	EmptyElementsSeletedArray();
}

void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SwitchCameraLocation();
}

void ABasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (Input != nullptr)
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayer::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasePlayer::Look);
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		Input->BindAction(AttackAction, ETriggerEvent::Started, this, &ABasePlayer::AttackStarted);
		Input->BindAction(AttackAction, ETriggerEvent::Ongoing, this, &ABasePlayer::AttackOngoing);
		Input->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ABasePlayer::AttackTriggered);
		Input->BindAction(AttackAction, ETriggerEvent::Canceled, this, &ABasePlayer::AttackTriggered);

		Input->BindAction(CastAction, ETriggerEvent::Started, this, &ABasePlayer::CastStarted);
		Input->BindAction(CastAction, ETriggerEvent::Ongoing, this, &ABasePlayer::CastOngoing);
		Input->BindAction(CastAction, ETriggerEvent::Triggered, this, &ABasePlayer::CastTriggered);
		Input->BindAction(CastAction, ETriggerEvent::Canceled, this, &ABasePlayer::CastTriggered);

		Input->BindAction(ElementSelectAction1, ETriggerEvent::Started, this, &ABasePlayer::ElementSelectAction1Started);
		Input->BindAction(ElementSelectAction2, ETriggerEvent::Started, this, &ABasePlayer::ElementSelectAction2Started);
		Input->BindAction(ElementSelectAction3, ETriggerEvent::Started, this, &ABasePlayer::ElementSelectAction3Started);
		Input->BindAction(ElementSelectAction4, ETriggerEvent::Started, this, &ABasePlayer::ElementSelectAction4Started);

		Input->BindAction(MouseWheelAction, ETriggerEvent::Triggered, this, &ABasePlayer::MouseWheelTriggered);
		Input->BindAction(SubSkill1Action, ETriggerEvent::Started, this, &ABasePlayer::SubSkill1Started);
		Input->BindAction(SubSkill1Action, ETriggerEvent::Ongoing, this, &ABasePlayer::SubSkill1Ongoing);
		Input->BindAction(SubSkill1Action, ETriggerEvent::Triggered, this, &ABasePlayer::SubSkill1Triggered);
		Input->BindAction(SubSkill2Action, ETriggerEvent::Started, this, &ABasePlayer::SubSkill2Started);
		Input->BindAction(SubSkill2Action, ETriggerEvent::Ongoing, this, &ABasePlayer::SubSkill2Ongoing);
		Input->BindAction(SubSkill2Action, ETriggerEvent::Triggered, this, &ABasePlayer::SubSkill2Triggered);
	}
}

void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();

	// 향상된 입력 매핑 컨텍스트로 변경
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem != nullptr)
		{
			Subsystem->AddMappingContext(KBMMappingContext, 0);
		}
	}

	OriginSpringArmLength = SpringArm->TargetArmLength;
	OriginCameraLocation = ViewCamera->GetRelativeLocation();
	
	FloorAimingActor = SpawnAimingActor(FloorAimingClass);
	FlyAimingActor = SpawnAimingActor(FlyAimingClass);
	CurrMagicCircleDist = MagicCircleRange;

	InitPlayerOverlay();
	UpdateElementSlotUI();
}

void ABasePlayer::InitMagicCircleDistVariationSpeed()
{
	MagicCircleDistVariationSpeed = 1.0f;
}

void ABasePlayer::IncreaseMagicCircleDistVariationSpeed()
{
	MagicCircleDistVariationSpeed = FMath::Clamp(MagicCircleDistVariationSpeed * 1.1f, 0, 10.0f);
}

void ABasePlayer::SetCurrMagicCircleDist(float Value)
{
	CurrMagicCircleDist = FMath::Clamp(Value, MagicCircleCastableRange, MagicCircleRange);
}

void ABasePlayer::Move(const FInputActionInstance& Instance)
{
	FVector2D MovementVector = Instance.GetValue().Get<FVector2d>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection, MovementVector.Y);
	}
}

void ABasePlayer::Look(const FInputActionInstance& Instance)
{
	FVector2D LookVector = Instance.GetValue().Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
}

void ABasePlayer::AttackStarted(const FInputActionInstance& Instance)
{
	if (PlayerActionState == EPlayerActionState::EPAS_Unoccupied)
	{
		PlayerActionState = EPlayerActionState::EPAS_Casting;
		CastedMagic = ECastedMagic::ECM_MagicBullet;
		CameraState = EPlayerCameraState::EPCS_ZoomIn;
	}
}

void ABasePlayer::AttackOngoing(const FInputActionInstance& Instance)
{
	if (CastedMagic == ECastedMagic::ECM_MagicBullet && IsCoolDown(MagicBulletTimer))
	{
		FVector MagicCircleLocation;
		FVector OffsetVector;
		OffsetVector.X = MagicBulletLocationOffset.X;
		OffsetVector.Y = FMath::RandRange(-MagicBulletLocationOffset.Y, MagicBulletLocationOffset.Y);
		OffsetVector.Z = FMath::RandRange(-MagicBulletLocationOffset.Z, MagicBulletLocationOffset.Z);
		if (LocateCharacterFrontMagicCircle(OffsetVector, MagicCircleLocation))
		{
			FRotator SpawnRotator = GetCharacterFrontMagicCircleRotator();
			SpawnMagicCircle(MagicCircleLocation, SpawnRotator, MagicBulletCircle);
			ABaseMagicProjectile* MagicBullet = Cast<ABaseMagicProjectile>(SpawnMagicActor(MagicCircleLocation, SpawnRotator, MagicBulletClass));
			if (MagicBullet)
			{
				MagicBullet->SetProjectileRange(MagicBulletRange);
				MagicBullet->SetDamage(MagicBulletDamage);
			}
			GetWorldTimerManager().SetTimer(MagicBulletTimer, MagicBulletCoolTime, false);
		}
	}
}

void ABasePlayer::AttackTriggered(const FInputActionInstance& Instance)
{
	if (PlayerActionState == EPlayerActionState::EPAS_Casting && CastedMagic == ECastedMagic::ECM_MagicBullet)
	{
		PlayerActionState = EPlayerActionState::EPAS_Unoccupied;
		CastedMagic = ECastedMagic::ECM_None;
		CameraState = EPlayerCameraState::EPCS_ZoomOut;
	}
}

void ABasePlayer::CastStarted(const FInputActionInstance& Instance)
{
	if (PlayerActionState == EPlayerActionState::EPAS_Unoccupied && IsElementSeleted())
	{
		switch (GetSelectedElement(0) | GetSelectedElement(1))
		{
		case EFourElement::EPE_Ignis:
			CastedMagic = ECastedMagic::ECM_II;
			break;
		case EFourElement::EPE_Aqua:
			CastedMagic = ECastedMagic::ECM_AA;
			break;
		case EFourElement::EPE_Ventus:
			CastedMagic = ECastedMagic::ECM_VV;
			break; 
		case EFourElement::EPE_Terra:
			CastedMagic = ECastedMagic::ECM_TT;
			break;
		case EFourElement::EPE_Ignis | EFourElement::EPE_Ventus:
			CastedMagic = ECastedMagic::ECM_IV;
			break;
		case EFourElement::EPE_Ventus | EFourElement::EPE_Aqua:
			CastedMagic = ECastedMagic::ECM_VA;
			break;
		case EFourElement::EPE_Aqua | EFourElement::EPE_Terra:
			CastedMagic = ECastedMagic::ECM_AT;
			break;
		case EFourElement::EPE_Terra | EFourElement::EPE_Ignis:
			CastedMagic = ECastedMagic::ECM_TI;
			break;
		default:
			SCREEN_LOG_NONE_KEY(TEXT("Not Castable Combination!"));
			return;
			break;
		}
		PlayerActionState = EPlayerActionState::EPAS_Casting;
		CameraState = EPlayerCameraState::EPCS_ZoomIn;
	}
}

void ABasePlayer::CastOngoing(const FInputActionInstance& Instance)
{
	if (PlayerActionState == EPlayerActionState::EPAS_Casting)
	{
		FVector AimingLocation;
		switch (CastedMagic)
		{
		case ECastedMagic::ECM_II:
			ShowFloorAimingCircle();
			break;
		case ECastedMagic::ECM_AA:
			break;
		case ECastedMagic::ECM_VV:
			break;
		case ECastedMagic::ECM_TT:
			ShowFlyAimingCircle();
			break;
		case ECastedMagic::ECM_IV:
			break;
		case ECastedMagic::ECM_VA:
			ShowFloorAimingCircle();
			break;
		case ECastedMagic::ECM_AT:
			break;
		case ECastedMagic::ECM_TI:
			ShowFloorAimingCircle();
			break;
		default:
			break;
		}
	}
}

void ABasePlayer::CastTriggered(const FInputActionInstance& Instance)
{
	if (PlayerActionState == EPlayerActionState::EPAS_Casting)
	{
		if (PlayerActionState == EPlayerActionState::EPAS_Casting)
		{
			switch (CastedMagic)
			{
			case ECastedMagic::ECM_II:
				MagicII_FlameStrike();
				break;
			case ECastedMagic::ECM_AA:
				MagicAA_HealOverTime();
				break;
			case ECastedMagic::ECM_VV:
				MagicVV_Piercing();
				break;
			case ECastedMagic::ECM_TT:
				MagicTT_Portal();
				break;
			case ECastedMagic::ECM_IV:
				MagicIV_Explosion();
				break;
			case ECastedMagic::ECM_VA:
				MagicVA_Tornado();
				break;
			case ECastedMagic::ECM_AT:
				MagicAT_Summon();
				break;
			case ECastedMagic::ECM_TI:
				MagicTI_Meteorite();
				break;
			default:
				break;
			}
		}
		CastEnd();
	}
}

void ABasePlayer::ElementSelectAction1Started(const FInputActionInstance& Instance)
{
	SelectElement(1);
}

void ABasePlayer::ElementSelectAction2Started(const FInputActionInstance& Instance)
{
	SelectElement(2);
}

void ABasePlayer::ElementSelectAction3Started(const FInputActionInstance& Instance)
{
	SelectElement(3);
}

void ABasePlayer::ElementSelectAction4Started(const FInputActionInstance& Instance)
{
	SelectElement(4);
}

void ABasePlayer::MouseWheelTriggered(const FInputActionInstance& Instance)
{
	float InputValue = Instance.GetValue().Get<float>();
	CurrMagicCircleDist = FMath::Clamp(CurrMagicCircleDist + (InputValue * 10), MagicCircleCastableRange, MagicCircleRange);
}

void ABasePlayer::SubSkill1Started(const FInputActionInstance& Instance)
{
	InitMagicCircleDistVariationSpeed();
}

void ABasePlayer::SubSkill1Ongoing(const FInputActionInstance& Instance)
{
	IncreaseMagicCircleDistVariationSpeed();
	SetCurrMagicCircleDist(CurrMagicCircleDist + MagicCircleDistVariationSpeed);
}

void ABasePlayer::SubSkill1Triggered(const FInputActionInstance& Instance)
{
	
}

void ABasePlayer::SubSkill2Started(const FInputActionInstance& Instance)
{
	InitMagicCircleDistVariationSpeed();
}

void ABasePlayer::SubSkill2Ongoing(const FInputActionInstance& Instance)
{
	IncreaseMagicCircleDistVariationSpeed();
	SetCurrMagicCircleDist(CurrMagicCircleDist - MagicCircleDistVariationSpeed);
}

void ABasePlayer::SubSkill2Triggered(const FInputActionInstance& Instance)
{
}

FVector ABasePlayer::GetCameraLookAtLocation()
{
	return ViewCamera->GetComponentLocation() + (ViewCamera->GetForwardVector() * LookAtOffset);
}

void ABasePlayer::SwitchCameraLocation()
{
	switch (CameraState)
	{
	case EPlayerCameraState::EPCS_ZoomOut:
		ZoomOutCamera();
		break;
	case EPlayerCameraState::EPCS_ZoomIn:
		ZoomInCamera();
		break;
	default:
		return;
		break;
	}
	ViewCamera->SetRelativeLocation(TargetCameraLocation);
}

void ABasePlayer::ZoomOutCamera()
{
	SpringArm->TargetArmLength = FMath::Lerp<float, float>(SpringArm->TargetArmLength, OriginSpringArmLength, CameraMoveRate);
	TargetCameraLocation = FMath::Lerp<FVector, float>(ViewCamera->GetRelativeLocation(), OriginCameraLocation, CameraMoveRate);
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void ABasePlayer::ZoomInCamera()
{
	SpringArm->TargetArmLength = FMath::Lerp<float, float>(SpringArm->TargetArmLength, ZoomSpringArmLength, CameraMoveRate);
	TargetCameraLocation = FMath::Lerp<FVector, float>(ViewCamera->GetRelativeLocation(), ZoomCameraLocation, CameraMoveRate);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
}

void ABasePlayer::ShakeCamera(float Power)
{
	FVector Offset;
	Offset.X = FMath::RandRange(-1.0f, 1.0f);
	Offset.Y = FMath::RandRange(-1.0f, 1.0f);
	Offset.Z = FMath::RandRange(-1.0f, 1.0f);
	Offset.Normalize();
	ViewCamera->SetRelativeLocation(TargetCameraLocation + (Offset * Power));
}

FVector ABasePlayer::GetChestLocation()
{
	return GetActorLocation() + FVector(0, 0, ChestLocationZOffset);
}

FVector ABasePlayer::GetCharacterFrontMagicCircle()
{
	return GetChestLocation() + ViewCamera->GetRightVector() * ZoomCameraLocation.Y + (GetActorForwardVector() * CharacterFrontCastableRange);
}

EFourElement ABasePlayer::GetSelectedElement(uint8 i)
{
	if ((i < 0 || i > ElementsSelectedArray.Num()) || (ElementsSelectedArray[i] < 0 || ElementsSelectedArray[i] > ElementsArray.Num()))
	{
		return EFourElement::EPE_None;
	}
	return ElementsArray[ElementsSelectedArray[i]];
}

void ABasePlayer::SetSelectedElement(uint8 i, EFourElement Element)
{
	if ((i < 0 || i > ElementsSelectedArray.Num()) && (ElementsSelectedArray[i] < 0 || ElementsSelectedArray[i] > ElementsArray.Num()))
	{
		return;
	}
	ElementsArray[ElementsSelectedArray[i]] = Element;
}

bool ABasePlayer::LocateCharacterFrontMagicCircle(FVector Offset, FVector& Location)
{
	FVector Chest = GetChestLocation();
	FVector CastableStart = GetCharacterFrontMagicCircle();
	FVector Dummy;
	if (IsBlocked(Chest, CastableStart, Dummy))
	{
		return false;
	}
	FRotator Rotator = GetCharacterFrontMagicCircleRotator();
	FVector x = UKismetMathLibrary::GetForwardVector(Rotator);
	FVector y = UKismetMathLibrary::GetRightVector(Rotator);
	FVector z = UKismetMathLibrary::GetUpVector(Rotator);
	Location = CastableStart + x * Offset.X + y * Offset.Y + z * Offset.Z;
	return true;
}

bool ABasePlayer::LocateFloorMagicCircle(FVector Offset, FVector& Location)
{
	FVector Start = ViewCamera->GetComponentLocation() + ViewCamera->GetForwardVector() * MagicCircleCastableRange;
	FVector Block;
	if (IsBlocked(ViewCamera->GetComponentLocation(), Start, Block))
	{
		Location = Block;
		return false;
	}
	FVector End = Start + ViewCamera->GetForwardVector() * (Offset.X + MagicCircleRange) + ViewCamera->GetRightVector() * Offset.Y + ViewCamera->GetUpVector() * Offset.Z;
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult HitResult;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	UKismetSystemLibrary::BoxTraceSingleForObjects(
		this,
		Start - ViewCamera->GetUpVector() * FlyMagicCircleBoxTraceHalf.Z,
		End - ViewCamera->GetUpVector() * FlyMagicCircleBoxTraceHalf.Z,
		FlyMagicCircleBoxTraceHalf,
		FRotator::ZeroRotator,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);

	if (HitResult.GetActor())
	{
		Location = HitResult.ImpactPoint + FVector::ZAxisVector;
		if (HitResult.ImpactNormal.Dot(FVector::ZAxisVector) < FMath::Cos(FMath::DegreesToRadians(45)))
		{
			return false;
		}
		return true;
	}

	Location = End;
	return false;
}

bool ABasePlayer::LocateFlyMagicCircle(FVector Offset, FVector& Location)
{
	FVector Start = ViewCamera->GetComponentLocation() + ViewCamera->GetForwardVector() * MagicCircleCastableRange;
	FVector Block;
	if (IsBlocked(ViewCamera->GetComponentLocation(), Start, Block))
	{
		Location = Block;
		return false;
	}
	FVector End = Start + ViewCamera->GetForwardVector() * Offset.X + ViewCamera->GetRightVector() * Offset.Y + ViewCamera->GetUpVector() * Offset.Z;
	if (IsBlocked(Start, End, Block))
	{
		Location = Block + FVector::ZAxisVector;
	}
	else
	{
		Location = End;
	}

	return true;
}
/// <summary>
/// LineTrace로 두 위치 사이에 Actor가 있는지 확인한다.
/// </summary>
/// <param name="Start">첫번째 위치</param>
/// <param name="End">두번째 위치</param>
/// <param name="BlockedLocation">막힌 위치를 반환한다. 막히지 않았을 경우 End위치를 반환한다.</param>
/// <returns></returns>
bool ABasePlayer::IsBlocked(FVector Start, FVector End, FVector& BlockedLocation, TArray<AActor*> ActorsToIgnore)
{
	ActorsToIgnore.Add(this);
	FHitResult HitResult;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		Start,
		End,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);
	if (HitResult.GetActor())
	{
		BlockedLocation = HitResult.ImpactPoint;
		return true;
	}
	BlockedLocation = End;
	return false;
}

bool ABasePlayer::IsCoolDown(FTimerHandle& CoolTimer)
{
	return GetWorldTimerManager().GetTimerRemaining(CoolTimer) < 0;
}

bool ABasePlayer::IsElementSeleted()
{
	return ElementsSelectedArray[0] != -1 && ElementsSelectedArray[1] != -1;
}

FRotator ABasePlayer::GetCharacterFrontMagicCircleRotator()
{
	FVector x = GetCameraLookAtLocation() - GetCharacterFrontMagicCircle();
	FVector y = ViewCamera->GetRightVector();
	FVector z = x.Cross(y);
	y = z.Cross(x);
	FRotator Rotator = FMatrix(x, y, z, FVector::ZeroVector).Rotator();
	return Rotator;
}

FRotator ABasePlayer::GetFloorMagicCircleRotator()
{
	return FRotator();
}

FRotator ABasePlayer::GetFlyMagicCircleRotator()
{
	return ViewCamera->GetComponentRotation();
}

void ABasePlayer::InitElementsArray(EFourElement First, EFourElement Second, EFourElement Third, EFourElement Forth)
{
	ElementsArray.SetNum(4);
	ElementsArray[0] = First;
	ElementsArray[1] = Second;
	ElementsArray[2] = Third;
	ElementsArray[3] = Forth;
}

void ABasePlayer::InitElementsReadyArray(EFourElement First, EFourElement Second, EFourElement Third, EFourElement Forth)
{
	ElementsReadyArray.SetNum(4);
	ElementsReadyArray[0] = First;
	ElementsReadyArray[1] = Second;
	ElementsReadyArray[2] = Third;
	ElementsReadyArray[3] = Forth;
}

void ABasePlayer::EmptyElementsSeletedArray()
{
	ElementsSelectedArray.SetNum(2);
	ElementsSelectedArray[0] = -1;
	ElementsSelectedArray[1] = -1;
}

void ABasePlayer::SelectElement(uint8 Index)
{
	if (ElementsSelectedArray[0] == Index - 1 || CastedMagic > ECastedMagic::ECM_MagicBullet) return;
	int n = ElementsSelectedArray.Num();
	for (int i = 1; i < n; ++i)
	{
		ElementsSelectedArray[n - i] = ElementsSelectedArray[n - i - 1];
	}
	ElementsSelectedArray[0] = Index - 1;
	UpdateElementSlotUI();
}

void ABasePlayer::UseSelectedElements()
{
	if (!IsElementSeleted()) return;
	TArray<EFourElement> Selected;
	int n = ElementsSelectedArray.Num();
	int m = ElementsReadyArray.Num();
	for (int i = 0; i < n; ++i)
	{
		Selected.Add(GetSelectedElement(n - i - 1));
		SetSelectedElement(n - i - 1, ElementsReadyArray[i]);
	}
	for (int i = n; i < m; ++i)
	{
		ElementsReadyArray[i - n] = ElementsReadyArray[i];
	}
	for (int i = 0; i < n; ++i)
	{
		ElementsReadyArray[m - n + i] = Selected[i];
	}
	EmptyElementsSeletedArray();
	UpdateElementSlotUI();
}

void ABasePlayer::MagicII_FlameStrike()
{
	FVector CircleLocation;
	if (LocateFloorMagicCircle(FVector::ZeroVector, CircleLocation))
	{
		SpawnMagicCircle(CircleLocation, FRotator::ZeroRotator, FlameStrikeCircle);
		ABaseMagic* FlameStrike = SpawnMagicActor(CircleLocation, FRotator::ZeroRotator, FlameStrikeClass);
		if (FlameStrike)
		{
			FlameStrike->SetDamage(FlameStrikeDamage);
		}
	}
}

void ABasePlayer::MagicAA_HealOverTime()
{
	FVector CircleLocation = GetUnderCharacterFeetLocation();
	SpawnMagicCircle(CircleLocation, FRotator::ZeroRotator, HealOverTimeCircle);
	HealOverTimeCharacter(AmountOfHealPerTime, HealCount, HealDelay);
}

void ABasePlayer::MagicVV_Piercing()
{
	FVector CircleLocation;
	if (LocateCharacterFrontMagicCircle(FVector::ZeroVector, CircleLocation))
	{
		FRotator CircleRotator = GetCharacterFrontMagicCircleRotator();
		SpawnMagicCircle(CircleLocation, CircleRotator, PiercingCircle);
		APiercing* Piercing = Cast<APiercing>(SpawnMagicActor(CircleLocation, CircleRotator, PiercingClass));
		if (Piercing)
		{
			Piercing->SetDamage(PiercingDamage);
			Piercing->SetProjectileRange(PiercingRange);
			Piercing->SetPiercingDelay(PiercingDelay);
		}
	}
}

void ABasePlayer::MagicTT_Portal()
{
	FVector MagicLocation;
	if (LocateFlyMagicCircle(FVector::XAxisVector * CurrMagicCircleDist, MagicLocation))
	{
		FRotator MagicRotator = GetFlyMagicCircleRotator();
		MagicRotator.Pitch = 0;
		APortal* Portal1 = Cast<APortal>(SpawnMagicActor(MagicLocation, MagicRotator, PortalClass));
		APortal* Portal2 = Cast<APortal>(SpawnMagicActor(MagicLocation, MagicRotator, PortalClass));
		Portal1->SetOutPortal(Portal2);
		Portal1->SetPortalLifeTime(PortalLifeTime);
		Portal2->SetOutPortal(Portal1);
		Portal2->SetPortalLifeTime(PortalLifeTime);

		Portal1->SetActorLocation(GetActorLocation() - FVector::ZAxisVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	}
}

void ABasePlayer::MagicIV_Explosion()
{
}

void ABasePlayer::MagicVA_Tornado()
{
}

void ABasePlayer::MagicAT_Summon()
{
}

void ABasePlayer::MagicTI_Meteorite()
{
}

void ABasePlayer::CastEnd()
{
	PlayerActionState = EPlayerActionState::EPAS_Unoccupied;
	CastedMagic = ECastedMagic::ECM_None;
	CameraState = EPlayerCameraState::EPCS_ZoomOut;
	UseSelectedElements();
	FloorAimingActor->SetAimingMeshVisibility(false);
	FlyAimingActor->SetAimingMeshVisibility(false);
}

void ABasePlayer::UpdateElementSlotUI()
{
	TArray<EFourElement> SelectedArray;
	for (int i = 0; i < ElementsSelectedArray.Num(); ++i)
	{
		SelectedArray.Add(GetSelectedElement(i));
	}
	PlayerOverlay->SetElementSlots(ElementsArray, ElementsReadyArray, SelectedArray);
}

ABaseAiming* ABasePlayer::SpawnAimingActor(TSubclassOf<ABaseAiming> AimingClass)
{
	UWorld* World = GetWorld();
	if (World && AimingClass)
	{
		ABaseAiming* AimingActor = World->SpawnActor<ABaseAiming>(AimingClass, FVector::ZeroVector, FRotator::ZeroRotator);
		AimingActor->SetOwner(this);
		AimingActor->SetInstigator(this);
		AimingActor->SetAimingMeshVisibility(false);
		return AimingActor;
	}
	return nullptr;
}

UNiagaraComponent* ABasePlayer::SpawnMagicCircle(FVector Location, FRotator Rotator, UNiagaraSystem* MagicCircle)
{
	UWorld* World = GetWorld();
	if (World && MagicCircle)
	{
		return UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, MagicCircle ,Location, Rotator);
	}
	return nullptr;
}

ABaseMagic* ABasePlayer::SpawnMagicActor(FVector Location, FRotator Rotator, TSubclassOf<ABaseMagic> MagicClass)
{
	UWorld* World = GetWorld();
	if (World && MagicClass)
	{
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.Instigator = this;
		ActorSpawnParameters.Owner = this;
		ABaseMagic* Magic = World->SpawnActor<ABaseMagic>(MagicClass, Location, Rotator, ActorSpawnParameters);
		return Magic;
	}
	return nullptr;
}

void ABasePlayer::ShowFloorAimingCircle()
{
	FVector AimingLocation;
	if (!FloorAimingActor) return;
	if (LocateFloorMagicCircle(FVector::ZeroVector, AimingLocation))
	{
		FloorAimingActor->SetActorLocation(AimingLocation);
		FloorAimingActor->SetAimingMeshVisibility(true);
	}
	else
	{
		FloorAimingActor->SetAimingMeshVisibility(false);
	}
}

void ABasePlayer::ShowFlyAimingCircle()
{
	FVector AimingLocation;
	if (!FlyAimingActor) return;
	if (LocateFlyMagicCircle(FVector::XAxisVector * CurrMagicCircleDist, AimingLocation))
	{
		FlyAimingActor->SetActorLocation(AimingLocation);
		FlyAimingActor->SetActorRotation(GetFlyMagicCircleRotator());
		FlyAimingActor->SetAimingMeshVisibility(true);
	}
	else
	{
		FlyAimingActor->SetAimingMeshVisibility(false);
	}
}

void ABasePlayer::InitPlayerOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
		if (PlayerHUD)
		{
			PlayerOverlay = Cast<UPlayerOverlay>(PlayerHUD->GetPlayerOverlay());
		}
	}
}
