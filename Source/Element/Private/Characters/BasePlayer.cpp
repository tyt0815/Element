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
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	InitElementsArray(EPlayerElement::EPE_Ignis, EPlayerElement::EPE_Aqua, EPlayerElement::EPE_Ventus, EPlayerElement::EPE_Terra);
	InitElementsReadyArray(EPlayerElement::EPE_Ignis, EPlayerElement::EPE_Aqua, EPlayerElement::EPE_Ventus, EPlayerElement::EPE_Terra);
	InitElementsSeletedArray();
}

void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ZoomOutCamera();

	PlayerOverlay->SetElementSlots(ElementsArray, ElementsReadyArray, ElementsSelectedArray);
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
	// MagicBullet
	if (GetWorldTimerManager().GetTimerRemaining(MagicBulletTimer) < 0)
	{
		FVector OffsetVector;
		OffsetVector.X = MagicBulletLocationOffset.X;
		OffsetVector.Y = FMath::RandRange(-MagicBulletLocationOffset.Y, MagicBulletLocationOffset.Y);
		OffsetVector.Z = FMath::RandRange(-MagicBulletLocationOffset.Z, MagicBulletLocationOffset.Z);
		FVector SpawnLocation = GetMagicCircleMiddlePointLocation(OffsetVector);
		FRotator SpawnRotator = GetMagicCircleRotator();
		ActivateMagicCircle(SpawnLocation, SpawnRotator, MagicBulletRange, MagicBulletCircleClass);
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
	FVector FloorLocation;
	if (FindFloorMagicCircleLocation(GetMagicCircleMiddlePointLocation(FVector(GetCastableRange(MagicCircleRange), 0, 0)), FloorLocation))
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
}

FVector ABasePlayer::GetCameraLookAtLocation()
{
	return ViewCamera->GetComponentLocation() + (ViewCamera->GetForwardVector() * LookAtOffset); 
}

float ABasePlayer::GetSpringArmVelocity()
{
	return (OriginSpringArmLength - ZoomSpringArmLength) * CameraMoveRate;
}

FVector ABasePlayer::GetCameraVelocity()
{
	return (OriginCameraLocation - ZoomCameraLocation) * CameraMoveRate;
}

void ABasePlayer::ZoomOutCamera()
{
	if (PlayerActionState != EPlayerActionState::EPAS_Casting)
	{
		float SpringArmSpeed = GetSpringArmVelocity();
		FVector CameraSpeed = GetCameraVelocity();
		SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength + SpringArmSpeed, ZoomSpringArmLength, OriginSpringArmLength);
		ViewCamera->SetRelativeLocation(VectorClamp(ViewCamera->GetRelativeLocation() + CameraSpeed, OriginCameraLocation, ZoomCameraLocation));
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw = false;
	}
}

void ABasePlayer::ZoomInCamera()
{
	PlayerActionState = EPlayerActionState::EPAS_Casting;
	float SpringArmSpeed = GetSpringArmVelocity();
	FVector CameraSpeed = GetCameraVelocity();
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength - SpringArmSpeed, ZoomSpringArmLength, OriginSpringArmLength);
	ViewCamera->SetRelativeLocation(VectorClamp(ViewCamera->GetRelativeLocation() - CameraSpeed, OriginCameraLocation, ZoomCameraLocation));
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
		EDrawDebugTrace::ForOneFrame,
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
		EDrawDebugTrace::ForOneFrame,
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

void ABasePlayer::InitElementsArray(EPlayerElement First, EPlayerElement Second, EPlayerElement Third, EPlayerElement Forth)
{
	ElementsArray.SetNum(4);
	ElementsArray[0] = First;
	ElementsArray[1] = Second;
	ElementsArray[2] = Third;
	ElementsArray[3] = Forth;
}

void ABasePlayer::InitElementsReadyArray(EPlayerElement First, EPlayerElement Second, EPlayerElement Third, EPlayerElement Forth)
{
	ElementsReadyArray.SetNum(4);
	ElementsReadyArray[0] = First;
	ElementsReadyArray[1] = Second;
	ElementsReadyArray[2] = Third;
	ElementsReadyArray[3] = Forth;
}

void ABasePlayer::InitElementsSeletedArray()
{
	ElementsSelectedArray.SetNum(2);
	ElementsSelectedArray[0] = EPlayerElement::EPE_None;
	ElementsSelectedArray[1] = EPlayerElement::EPE_None;
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
