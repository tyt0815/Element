#include "Characters/BasePlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Controller.h"

#include "Element/DebugMacro.h"
#include "Magic/MagicCircle.h"
#include "Magic/BaseMagic.h"

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
}

void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ZoomOutCamera();
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
	if (GetWorldTimerManager().GetTimerRemaining(MagicBulletTimer) < 0)
	{
		FVector OffsetVector;
		OffsetVector.X = FMath::RandRange(-MagicBulletLocationOffset.X, MagicBulletLocationOffset.X);
		OffsetVector.Y = FMath::RandRange(-MagicBulletLocationOffset.Y, MagicBulletLocationOffset.Y);
		OffsetVector.Z = FMath::RandRange(-MagicBulletLocationOffset.Z, MagicBulletLocationOffset.Z);
		FVector SpawnLocation = GetFlyMagicCircleLocation(OffsetVector);
		FRotator SpawnRotator = GetFlyMagicCircleRotator();
		ActivateMagicCircle(SpawnLocation, SpawnRotator, MagicBulletRange, MagicBulletCircleClass);
		GetWorldTimerManager().SetTimer(MagicBulletTimer, MagicBulletCoolTime, false);
	}
}

void ABasePlayer::AttackTriggered(const FInputActionInstance& Instance)
{
	SCREEN_LOG(0, "AttackTriggered");
	PlayerActionState = EPlayerActionState::EPAS_Unoccupied;
}

void ABasePlayer::CastOngoing(const FInputActionInstance& Instance)
{

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
		ViewCamera->SetRelativeLocation(VectorClamp(ViewCamera->GetRelativeLocation() + CameraSpeed, ZoomCameraLocation, OriginCameraLocation));
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
	ViewCamera->SetRelativeLocation(VectorClamp(ViewCamera->GetRelativeLocation() - CameraSpeed, ZoomCameraLocation, OriginCameraLocation));
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
}

FVector ABasePlayer::GetFlyMagicCircleLocation(FVector& Offset)
{
	FRotator Rotator = GetFlyMagicCircleRotator();
	FVector x = FInverseRotationMatrix(Rotator).GetUnitAxis(EAxis::X);
	FVector y = FInverseRotationMatrix(Rotator).GetUnitAxis(EAxis::Y);
	FVector z = FInverseRotationMatrix(Rotator).GetUnitAxis(EAxis::Z);
	FVector Start = GetMagicCircleMiddlePointLocation();
	return Start + (x * Offset.X) + (y * Offset.Y) + (z * Offset.Z);
}

FRotator ABasePlayer::GetFlyMagicCircleRotator()
{
	FVector x = GetCameraLookAtLocation() - GetMagicCircleMiddlePointLocation();
	FVector y = ViewCamera->GetRightVector();
	FVector z = x.Cross(y);
	y = z.Cross(x);
	FMatrix RotationMatrix(x, y, z, FVector::ZeroVector);
	FRotator Rotator = RotationMatrix.Rotator();
	return Rotator;
}

FVector ABasePlayer::GetMagicCircleMiddlePointLocation()
{
	FVector Start = GetActorLocation() + FVector(0, 0, MagicCircleMiddlePointOffset.Z);
	FVector x = GetCameraLookAtLocation() - Start;
	x.Normalize();
	FVector y = ViewCamera->GetRightVector();
	FVector z = x.Cross(y);
	y = z.Cross(x);
	return Start + x * MagicCircleMiddlePointOffset.X;
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