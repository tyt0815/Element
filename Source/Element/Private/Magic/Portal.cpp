#include "Magic/Portal.h"
#include "Components/BoxComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "Element/DebugMacro.h"
#include "Characters/BasePlayer.h"
#include "Helper/MathHelper.h"

APortal::APortal()
{
	PrimaryActorTick.bCanEverTick = false;

	HitBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitBoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	HitBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);

	PortalBackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalBackMesh"));
	PortalBackMesh->SetupAttachment(GetRootComponent());
	PortalBackMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PortalFrontSceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PortalFrontSceneCapture"));
	PortalFrontSceneCapture->SetupAttachment(GetRootComponent());
	PortalFrontSceneCapture->PostProcessSettings.DynamicGlobalIlluminationMethod = EDynamicGlobalIlluminationMethod::Lumen;
	StaticMeshComponent->bHiddenInSceneCapture = true;
	PortalBackSceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PortalBackSceneCapture"));
	PortalBackSceneCapture->SetupAttachment(GetRootComponent());
	PortalBackSceneCapture->PostProcessSettings.DynamicGlobalIlluminationMethod = EDynamicGlobalIlluminationMethod::Lumen;
	PortalBackMesh->bHiddenInSceneCapture = true;
}

void APortal::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(PortalLifeTime);

	FVector Scale = GetActorScale3D();
	int PortalRenderWidth = Scale.X * 1000, PortalRenderHeight = Scale.Y * 1000;
	if (Scale.X > Scale.Y) PortalRenderHeight *= Scale.Y / Scale.X;
	else if (Scale.X < Scale.Y) PortalRenderWidth *= Scale.X / Scale.Y;
	PortalFrontDynamicMaterialInstance = StaticMeshComponent->CreateDynamicMaterialInstance(0);
	PortalFrontRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), PortalRenderWidth, PortalRenderHeight);
	PortalFrontSceneCapture->TextureTarget = PortalFrontRenderTarget;
	PortalFrontSceneCapture->CustomNearClippingPlane = FMath::Abs(PortalFrontSceneCapture->GetRelativeLocation().X);
	PortalBackDynamicMaterialInstance = PortalBackMesh->CreateDynamicMaterialInstance(0);
	PortalBackRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), PortalRenderWidth, PortalRenderHeight);
	PortalBackSceneCapture->TextureTarget = PortalBackRenderTarget;
	PortalBackSceneCapture->CustomNearClippingPlane = FMath::Abs(PortalBackSceneCapture->GetRelativeLocation().X);

	SetPortalRenderTarget();
}

void APortal::ChangePortalColor()
{
	static int8 PortalColorIndex = 0;
	static TArray<FColor> PortalColor = { FColor::Red, FColor::Cyan, FColor::Green, FColor::Yellow };
	if (PortalFrontDynamicMaterialInstance && PortalBackDynamicMaterialInstance)
	{
		PortalFrontDynamicMaterialInstance->SetVectorParameterValue(TEXT("PortalColor"), PortalColor[PortalColorIndex++]);
		PortalBackDynamicMaterialInstance->SetVectorParameterValue(TEXT("PortalColor"), PortalColor[PortalColorIndex++]);
	}
}

void APortal::SetOutPortal(APortal* Portal)
{
	OutPortal = Portal;
	SetPortalRenderTarget();
}

void APortal::SetPortalRenderTarget()
{
	if (OutPortal && OutPortal->GetPortalFrontRenderTarget() != nullptr && OutPortal->GetPortalBackRenderTarget() != nullptr)
	{
		ActorsToIgnore.AddUnique(OutPortal);
		PortalFrontDynamicMaterialInstance->SetTextureParameterValue(FName("RenderTarget"), OutPortal->GetPortalBackRenderTarget());
		PortalBackDynamicMaterialInstance->SetTextureParameterValue(FName("RenderTarget"), OutPortal->GetPortalFrontRenderTarget());
	}
	else
	{
		PortalFrontDynamicMaterialInstance->SetTextureParameterValue(FName("RenderTarget"), PortalFrontRenderTarget);
		PortalBackDynamicMaterialInstance->SetTextureParameterValue(FName("RenderTarget"), PortalBackRenderTarget);
		GetWorldTimerManager().SetTimerForNextTick(this, &APortal::SetPortalRenderTarget);
	}
}

void APortal::ActivatePortal_Implementation()
{
	Activated = true;
	HitBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	StaticMeshComponent->SetVisibility(true);
	PortalBackMesh->SetVisibility(true);
}

void APortal::DeactivatePortal_Implementation()
{
	Activated = false;
	HitBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetVisibility(false);
	PortalBackMesh->SetVisibility(false);
}

void APortal::TeleportActor(AActor* Actor)
{
	if (OutPortal == nullptr || Actor == nullptr) return;
	OutPortal->AddActorsToIgnore(Actor);

	FVector TeleportLocation = OutPortal->GetActorLocation() + Actor->GetActorLocation() - GetActorLocation();
	Actor->SetActorLocation(TeleportLocation, false, nullptr, ETeleportType::TeleportPhysics);

	FRotator TeleportRotation = OutPortal->GetActorRotation() - GetActorRotation();
	ABasePlayer* Player = Cast<ABasePlayer>(Actor);
	if (Player)
	{
		FRotator ControllerRotator = Player->GetController()->GetControlRotation() - Player->GetActorRotation();
		Player->AddActorWorldRotation(TeleportRotation, false, nullptr, ETeleportType::TeleportPhysics);
		ControllerRotator += Player->GetActorRotation();
		SCREEN_LOG(0, ControllerRotator.ToString());
		Player->GetController()->SetControlRotation(ControllerRotator);
		if (Player->GetPlayerActionState() == EPlayerActionState::EPAS_Lifting)
		{
			Player->GetTargetInteractiveActor()->SetActorLocation(
				Player->GetTargetInteractiveActor()->GetActorLocation() + (OutPortal->GetActorLocation() - GetActorLocation())
			);
		}
		Player->GetCharacterMovement()->Velocity = TeleportRotation.RotateVector(Player->GetCharacterMovement()->Velocity);
	}
	else
	{
		Actor->AddActorWorldRotation(TeleportRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}
	

	ActorsToIgnore.Remove(Actor);
}
void APortal::BeginBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Activated) return;
	while (true)
	{
		FHitResult HitResult;
		BoxTrace(HitResult);
		if (!HitResult.GetActor()) break;
		TeleportActor(HitResult.GetActor());
	}
}



void APortal::EndBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!Activated) return;
	Super::EndBoxOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	ActorsToIgnore.Remove(OtherActor);
}

void APortal::InitActorsToIgnore()
{
	ActorsToIgnore.Empty();
}

void APortal::InitHitTraceObjectTypes()
{
	HitTraceObjectTypes.Empty();
	HitTraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
	HitTraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
}
