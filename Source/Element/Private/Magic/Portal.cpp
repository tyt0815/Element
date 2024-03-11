#include "Magic/Portal.h"
#include "Components/BoxComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Element/DebugMacro.h"
#include "Characters/BasePlayer.h"
#include "Helper/MathHelper.h"

APortal::APortal()
{
	PrimaryActorTick.bCanEverTick = false;

	HitBoxComponent->SetupAttachment(StaticMeshComponent);
	HitBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitBoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	HitBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
	PortalFrontMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalFrontMesh"));
	PortalFrontMesh->SetupAttachment(StaticMeshComponent);
	PortalFrontMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->bHiddenInSceneCapture = true;
	PortalFrontMesh->bHiddenInSceneCapture = true;
	PortalFrontSceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PortalFrontSceneCapture"));
	PortalFrontSceneCapture->SetupAttachment(StaticMeshComponent);
	PortalBackSceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PortalBackSceneCapture"));
	PortalBackSceneCapture->SetupAttachment(PortalFrontMesh);
	
}

void APortal::BeginPlay()
{
	Super::BeginPlay();

	// Æ÷Å»ÀÇ º®°ú °ãÄ¥ °æ¿ì À§Ä¡¸¦ ÀçÁ¶Á¤ ÇØÁØ´Ù.
	TArray<FHitResult> HitResults;
	TArray<AActor*> ToIgnore;
	TArray<TEnumAsByte<EObjectTypeQuery>> HitBoxTraceObjectTypes;
	HitBoxTraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	HitBoxTraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	bool bMovedHorizontal = false;
	// À­ºÎºÐ°ú °ãÄ¥ °æ¿ì
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		this,
		GetHitBoxTraceDownLocation() + FVector::ZAxisVector,
		GetHitBoxTraceUpLocation(),
		GetHitBoxTraceDownToUpHalfSize(),
		GetActorRotation(),
		HitBoxTraceObjectTypes,
		false,
		ToIgnore,
		EDrawDebugTrace::None,
		HitResults,
		true
	);
	for (auto& HitResult : HitResults)
	{
		if (HitResult.GetActor())
		{
			if (MathHelper::GetAngleBetweenTwoVectorsByDegree(HitResult.ImpactNormal, GetActorUpVector()) > 170.0f)
			{
				ToIgnore.AddUnique(HitResult.GetActor());
				SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, HitResult.ImpactPoint.Z - (2 * StaticMeshComponent->GetRelativeLocation().Z)));
				break;
			}
		}
	}

	SetLifeSpan(PortalLifeTime);
}

void APortal::SetSceneCaptureRenderTarget(UTextureRenderTarget2D* FrontRenderTarget, UTextureRenderTarget2D* BackRenderTarget)
{
	SCREEN_LOG_NONE_KEY(FString("APortal::SetSceneCaptureRenderTarget: ") + GetName());
	if (PortalFrontSceneCapture) PortalFrontSceneCapture->TextureTarget = FrontRenderTarget;
	if (PortalBackSceneCapture) PortalBackSceneCapture->TextureTarget = BackRenderTarget;
}

void APortal::ChangePortalColor()
{
	static int8 PortalColorIndex = 0;
	static TArray<FColor> PortalColor = { FColor::Red, FColor::Cyan, FColor::Green, FColor::Yellow };
}

void APortal::ActivatePortal_Implementation()
{
	Activated = true;
	HitBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	StaticMeshComponent->SetVisibility(true);
	PortalFrontMesh->SetVisibility(true);
}

void APortal::DeactivatePortal_Implementation()
{
	Activated = false;
	HitBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetVisibility(false);
	PortalFrontMesh->SetVisibility(false);
}

void APortal::TeleportActor(AActor* Actor)
{
	if (OutPortal == nullptr || Actor == nullptr) return;
	if (Cast<APortal>(Actor) != nullptr) return;
	OutPortal->AddActorsToIgnore(Actor);

	FVector TeleportLocation = OutPortal->GetActorLocation() + Actor->GetActorLocation() - GetActorLocation();
	Actor->SetActorLocation(OutPortal->StaticMeshComponent->GetComponentLocation(), false, nullptr, ETeleportType::TeleportPhysics);
	Actor->SetActorLocation(TeleportLocation, true, nullptr, ETeleportType::TeleportPhysics);

	FRotator TeleportRotation = OutPortal->GetActorRotation() - GetActorRotation();
	ABasePlayer* Player = Cast<ABasePlayer>(Actor);
	if (Player)
	{
		FRotator ControllerRotator = Player->GetController()->GetControlRotation() - Player->GetActorRotation();
		Player->AddActorWorldRotation(TeleportRotation, false, nullptr, ETeleportType::TeleportPhysics);
		ControllerRotator += Player->GetActorRotation();
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

	TArray<FHitResult> HitResults;
	BoxTraceMulti(HitResults);
	TArray<AActor*> TeleportedActors;
	for (auto& HitResult : HitResults)
	{
		if (TeleportedActors.Find(HitResult.GetActor()) == INDEX_NONE)
		{
			TeleportedActors.AddUnique(HitResult.GetActor());
			TeleportActor(HitResult.GetActor());
		}
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