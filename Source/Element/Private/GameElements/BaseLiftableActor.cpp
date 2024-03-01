#include "GameElements/BaseLiftableActor.h"
#include "Kismet/KismetMathLibrary.h"

#include "Characters/BasePlayer.h"
#include "Helper/MathHelper.h"

ABaseLiftableActor::ABaseLiftableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMesh"));
	SetRootComponent(RootMesh);
	RootMesh->SetSimulatePhysics(true);
	RootMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void ABaseLiftableActor::Tick(float DeltaTime)
{	
	MoveToLiftedLocation(DeltaTime);
	RotateToLiftedRotation(DeltaTime);
	if (Lifted && FVector::Distance(LiftingActor->GetActorLocation() + LiftingOffset, GetActorLocation()) > LiftingDistance * 1.3f) LayDown();
}

void ABaseLiftableActor::RotateToLiftedRotation(float DeltaTime)
{
	// Location이 설정된 후, EndOverlap되어 LiftingActor == nullptr이 될 수도 있음.
	if (Lifted && LiftingActor)
	{
		SetActorRotation(FMath::LerpStable(GetActorRotation(), LiftingActor->GetCameraRotation(), DeltaTime * RotateLerpAlpha), ETeleportType::ResetPhysics);
	}
}

void ABaseLiftableActor::MoveToLiftedLocation(float DeltaTime)
{
	if (Lifted && LiftingActor)
	{
		FVector TargetLocation = LiftingActor->GetActorLocation() + LiftingOffset;
		FVector Forward = LiftingActor->GetCameraForwardVector();	
		TargetLocation += (Forward * LiftingDistance);
		SetActorLocation(FMath::LerpStable(GetActorLocation(), TargetLocation, DeltaTime * MoveLerpAlpha), true, nullptr, ETeleportType::ResetPhysics);
	}
}

void ABaseLiftableActor::Interact_Implementation(AActor* InteractingActor)
{
	// Lay Down
	if (Lifted)
	{
		LayDown();
	}
	// Lift up
	else
	{
		LiftingActor = Cast<ABasePlayer>(InteractingActor);
		if (LiftingActor == nullptr) return;

		LiftingOffset = LiftingActor->GetCameraRelativeLocation();
		RootMesh->SetSimulatePhysics(false);
		Lifted = true;
		LiftingActor->SetPlayerActionState(EPlayerActionState::EPAS_Lifting);
	}
}

FString ABaseLiftableActor::GetInteractionHint_Implementation() const
{
	return Lifted ? FString("Place") : FString("Lift");
}

void ABaseLiftableActor::LayDown()
{
	if (LiftingActor)
	{
		LiftingActor->SetPlayerActionState(EPlayerActionState::EPAS_Unoccupied);
		LiftingActor = nullptr;
	}
	RootMesh->SetSimulatePhysics(true);
	Lifted = false;
}
