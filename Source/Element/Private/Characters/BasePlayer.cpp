#include "Characters/BasePlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Controller.h"

#include "Element/DebugMacro.h"
#include "Magic/MagicCircle.h"
#include "Magic/BaseMagic.h"
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

	ZoomOutCamera();
	SCREEN_LOG(0, FString::SanitizeFloat(SpringArm->TargetArmLength));
	SCREEN_LOG(1, ViewCamera->GetRelativeLocation().ToString());
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

		Input->BindAction(AttackAction, ETriggerEvent::Ongoing, this, &ABasePlayer::AttackOngoing);
		Input->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ABasePlayer::AttackTriggered);

		Input->BindAction(CastAction, ETriggerEvent::Ongoing, this, &ABasePlayer::CastOngoing);
		Input->BindAction(CastAction, ETriggerEvent::Triggered, this, &ABasePlayer::CastTriggered);

		Input->BindAction(ElementSelectAction1, ETriggerEvent::Started, this, &ABasePlayer::ElementSelectAction1Started);
		Input->BindAction(ElementSelectAction2, ETriggerEvent::Started, this, &ABasePlayer::ElementSelectAction2Started);
		Input->BindAction(ElementSelectAction3, ETriggerEvent::Started, this, &ABasePlayer::ElementSelectAction3Started);
		Input->BindAction(ElementSelectAction4, ETriggerEvent::Started, this, &ABasePlayer::ElementSelectAction4Started);
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

	InitPlayerOverlay();
	UpdateElementSlotUI();
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

void ABasePlayer::AttackOngoing(const FInputActionInstance& Instance)
{
	ZoomInCamera();
	FVector MagicCircleLocation;
	FVector OffsetVector;
	OffsetVector.X = MagicBulletLocationOffset.X;
	OffsetVector.Y = FMath::RandRange(-MagicBulletLocationOffset.Y, MagicBulletLocationOffset.Y);
	OffsetVector.Z = FMath::RandRange(-MagicBulletLocationOffset.Z, MagicBulletLocationOffset.Z);
	if (IsCoolDown(MagicBulletTimer) && LocateCharacterFrontMagicCircle(OffsetVector, MagicCircleLocation))
	{
		FRotator SpawnRotator = GetCharacterFrontMagicCircleRotator();
		ActivateMagicCircle(MagicCircleLocation, SpawnRotator, MagicBulletRange, MagicBulletCircleClass);
		GetWorldTimerManager().SetTimer(MagicBulletTimer, MagicBulletCoolTime, false);
	}
}

void ABasePlayer::AttackTriggered(const FInputActionInstance& Instance)
{
	PlayerActionState = EPlayerActionState::EPAS_Unoccupied;
}

void ABasePlayer::CastOngoing(const FInputActionInstance& Instance)
{
	ZoomInCamera();
	
	PlayerActionState = EPlayerActionState::EPAS_Casting;
	FVector FloorLocation;
	/*if (FindFloorMagicCircleLocation(GetMagicCircleMiddlePointLocation(FVector(GetCastableRange(MagicCircleRange), 0, 0)), FloorLocation))
	{
		DRAW_SPHERE_SINGLE_FRAME(FloorLocation);
	}*/
	if (LocateFloorMagicCircle(FVector(MagicCircleRange, 0.0f, 0.0f), FloorLocation))
	{
		DRAW_SPHERE_SINGLE_FRAME(FloorLocation);
	}
}

void ABasePlayer::CastTriggered(const FInputActionInstance& Instance)
{
	ZoomOutCamera();
	FVector Offset(GetCastableRange(MagicCircleRange), 0, 0);
	FVector FlyLocation = GetMagicCircleMiddlePointLocation(Offset);
	FVector FloorLocation;
	if(FindFloorMagicCircleLocation(GetMagicCircleMiddlePointLocation(FVector(GetCastableRange(MagicCircleRange), 0, 0)), FloorLocation))
	{
		DRAW_SPHERE_COLOR(FloorLocation, FColor::Green);
	}

	PlayerActionState = EPlayerActionState::EPAS_Unoccupied;

	if (ElementsSelectedArray[ElementsSelectedArray.Num() - 1] != -1)
	{
		UseSelectedElements();
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

FVector ABasePlayer::GetCameraLookAtLocation()
{
	return ViewCamera->GetComponentLocation() + (ViewCamera->GetForwardVector() * LookAtOffset);
}

void ABasePlayer::ZoomOutCamera()
{
	if (PlayerActionState != EPlayerActionState::EPAS_Casting)
	{
		SpringArm->TargetArmLength = FMath::Lerp<float, float>(SpringArm->TargetArmLength, OriginSpringArmLength, CameraMoveRate);
		ViewCamera->SetRelativeLocation(FMath::Lerp<FVector, float>(ViewCamera->GetRelativeLocation(), OriginCameraLocation, CameraMoveRate));
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw = false;
	}
}

void ABasePlayer::ZoomInCamera()
{
	SpringArm->TargetArmLength = FMath::Lerp<float, float>(SpringArm->TargetArmLength, ZoomSpringArmLength, CameraMoveRate);
	ViewCamera->SetRelativeLocation(FMath::Lerp<FVector, float>(ViewCamera->GetRelativeLocation(), ZoomCameraLocation, CameraMoveRate));
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
}

/*
ActorLocation에서 Z축방향으로 어느정도 더한 위치(Chest쯤)
*/
FVector ABasePlayer::GetMagicCircleMiddlePointLocation()
{
	return GetActorLocation() + FVector(0, 0, MagicCircleMiddlePointOffset);
}

/*
ActorLocation에서 Z축방향으로 어느정도 더한 위치(Chest쯤)에서 LookAt방향으로 Offset만큼 더한 위치(LookAt - Chest벡터를 x축으로 하는 좌표계 기준)
*/
FVector ABasePlayer::GetMagicCircleMiddlePointLocation(FVector Offset)
{
	FRotator Rotator = GetMagicCircleRotator();
	FVector x = UKismetMathLibrary::GetForwardVector(Rotator);
	FVector y = UKismetMathLibrary::GetRightVector(Rotator);
	FVector z = UKismetMathLibrary::GetUpVector(Rotator);
	FVector Start = GetMagicCircleMiddlePointLocation();
	return Start + (x * Offset.X) + (y * Offset.Y) + (z * Offset.Z);
}

FRotator ABasePlayer::GetMagicCircleRotator()
{
	FVector x = GetCameraLookAtLocation() - GetMagicCircleMiddlePointLocation();
	FVector y = ViewCamera->GetRightVector();
	FVector z = x.Cross(y);
	y = z.Cross(x);
	FMatrix RotationMatrix(x, y, z, FVector::ZeroVector);
	FRotator Rotator = RotationMatrix.Rotator();
	return Rotator;
}

float ABasePlayer::GetCastableRange(float Range)
{
	FVector LineStart = GetMagicCircleMiddlePointLocation();
	FVector Offset(Range, 0, 0);
	FVector LineEnd = GetMagicCircleMiddlePointLocation(Offset);
	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingle(
		this,
		LineStart,
		LineEnd,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);
	if (HitResult.GetActor() != nullptr)
	{
		return (HitResult.Location - LineStart).Size();
	}
	return Range;
}

bool ABasePlayer::FindFloorMagicCircleLocation(FVector FlyLocation, FVector& FloorLocation)
{
	FVector Offset(0, 0, 0);
	FVector Start = GetMagicCircleMiddlePointLocation(Offset) - FVector(0, 0, 25.0f);
	FVector End = FlyLocation - FVector(0, 0, 25.0f);
	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(0, 0, 25),
		FRotator::ZeroRotator,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);
	bool IsFloor = HitResult.Normal.Dot(FVector::ZAxisVector) <  1 / FMath::Sqrt(2.0f) ? false : true;
	if (HitResult.GetActor() != nullptr && IsFloor)
	{
		FloorLocation = HitResult.ImpactPoint;
		return true;
	}

	return false;
}

FVector ABasePlayer::GetChestLocation()
{
	return GetActorLocation() + FVector(0, 0, ChestLocationZOffset);
}

bool ABasePlayer::LocateCharacterFrontMagicCircle(FVector Offset, FVector& Location)
{
	FVector Chest = GetChestLocation();
	FRotator Rotator = GetCharacterFrontMagicCircleRotator();
	FVector x = UKismetMathLibrary::GetForwardVector(Rotator);
	FVector y = UKismetMathLibrary::GetRightVector(Rotator);
	FVector z = UKismetMathLibrary::GetUpVector(Rotator);
	FVector CastableStart = Chest + x * CharacterFrontCastableRange;
	FVector Dummy;
	if (IsBlocked(Chest, CastableStart, Dummy))
	{
		return false;
	}
	Location = CastableStart + x * Offset.X + y * Offset.Y + z * Offset.Z;
	return true;
}

bool ABasePlayer::LocateFloorMagicCircle(FVector Offset, FVector& Location)
{
	FVector Start = ViewCamera->GetComponentLocation() + ViewCamera->GetForwardVector() * FlyCastableRange;
	FVector Block;
	if (IsBlocked(ViewCamera->GetComponentLocation(), Start, Block))
	{
		Location = Block;
		return false;
	}
	FVector End = Start + ViewCamera->GetForwardVector() * Offset.X + ViewCamera->GetRightVector() * Offset.Y + ViewCamera->GetUpVector() * Offset.Z;
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult HitResult;
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start - ViewCamera->GetUpVector() * FlyMagicCircleBoxTraceHalf.Z,
		End - ViewCamera->GetUpVector() * FlyMagicCircleBoxTraceHalf.Z,
		FlyMagicCircleBoxTraceHalf,
		FRotator::ZeroRotator,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);

	if (HitResult.GetActor())
	{
		Location = HitResult.ImpactPoint;
		if (HitResult.ImpactNormal.Dot(FVector::ZAxisVector) < FMath::Cos(FMath::DegreesToRadians(45)))
		{
			return false;
		}
		return true;
	}

	Location = End;
	return false;
}

bool ABasePlayer::LocateTopDownMagicCircle(FVector Offset, FVector& Location)
{
	return false;
}

bool ABasePlayer::LocateFlyMagicCircle(FVector Offset, FVector& Location)
{
	return false;
}
/// <summary>
/// LineTrace로 두 위치 사이에 Actor가 있는지 확인한다.
/// </summary>
/// <param name="Start">첫번째 위치</param>
/// <param name="End">두번째 위치</param>
/// <param name="BlockedLocation">막힌 위치를 반환한다. 막히지 않았을 경우 End위치를 반환한다.</param>
/// <returns></returns>
bool ABasePlayer::IsBlocked(FVector Start, FVector End, FVector& BlockedLocation)
{
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingle(
		this,
		Start,
		End,
		ETraceTypeQuery::TraceTypeQuery1,
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

FRotator ABasePlayer::GetCharacterFrontMagicCircleRotator()
{
	FVector x = GetCameraLookAtLocation() - GetChestLocation();
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

FRotator ABasePlayer::GetTopDownMagicCircleLocation()
{
	return FRotator();
}

FRotator ABasePlayer::GetFlyMagicCircleRotator()
{
	return FRotator();
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
	if (ElementsSelectedArray[0] == Index - 1) return;
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

	TArray<EFourElement> Selected;
	int n = ElementsSelectedArray.Num();
	int m = ElementsReadyArray.Num();
	for (int i = 0; i < n; ++i)
	{
		Selected.Add(ElementsArray[ElementsSelectedArray[n - i - 1]]);
		ElementsArray[ElementsSelectedArray[n - i - 1]] = ElementsReadyArray[i];
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

void ABasePlayer::UpdateElementSlotUI()
{
	PlayerOverlay->SetElementSlots(ElementsArray, ElementsReadyArray, ElementsSelectedArray);
}

void ABasePlayer::ActivateMagicCircle(FVector Location, FRotator Rotator, float Range, const TSubclassOf<AMagicCircle>& MagicCircleClass)
{
	AMagicCircle* MagicCircle = nullptr;
	UWorld* World = GetWorld();
	if (World && MagicCircleClass)
	{
		MagicCircle = World->SpawnActor<AMagicCircle>(MagicCircleClass, Location, Rotator);
		if (MagicCircle != nullptr)
		{
			MagicCircle->SetOwner(this);
			MagicCircle->SetInstigator(this);
			MagicCircle->Activate(Location, Rotator, Range);
		}
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
