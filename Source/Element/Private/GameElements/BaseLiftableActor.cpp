#include "GameElements/BaseLiftableActor.h"

#include "Characters/BasePlayer.h"

ABaseLiftableActor::ABaseLiftableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh->SetSimulatePhysics(true);
	BaseMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void ABaseLiftableActor::Tick(float DeltaTime)
{
	MoveToLiftedLocation(DeltaTime);
	RotateToLiftedRotation(DeltaTime);
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
		FVector TargetLocation = LiftingActor->GetActorLocation() + (FVector::ZAxisVector * LiftingZOffset);
		FVector Forward = LiftingActor->GetCameraForwardVector();	
		TargetLocation += (Forward * GetInteractRangeRadius() * FMath::Clamp(LiftingDistance, 0.5f, 1.0f));
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

		BaseMesh->SetSimulatePhysics(false);
		Lifted = true;
		DeactivateInteraction();
		LiftingActor->SetPlayerActionState(EPlayerActionState::EPAS_Lifting);
		LiftingActor->SetLiftedActor(this);
		BeUntargetedFromActor(LiftingActor);
	}
}

void ABaseLiftableActor::LayDown()
{
	if (LiftingActor)
	{
		LiftingActor->SetPlayerActionState(EPlayerActionState::EPAS_Unoccupied);
		LiftingActor = nullptr;
	}
	BaseMesh->SetSimulatePhysics(true);
	Lifted = false;
	ActivateInteraction();
	BeTargetedToOverlappingActors();
}

void ABaseLiftableActor::EndOverlapInteractRangeComponent_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::EndOverlapInteractRangeComponent_Implementation(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor == LiftingActor && Lifted && IsOutOfTheRangeOfInteraction(OtherActor)) LayDown();
}
