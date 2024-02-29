#include "Magic/Tornado.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Element/DebugMacro.h"

void ATornado::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> FlyActorsToIgnore;
	while (true)
	{
		FHitResult HitResult;
		BoxTrace(HitResult, FlyActorsToIgnore);
		if (HitResult.GetActor() == nullptr) break;
		if (HitResult.GetActor() == GetOwner())
		{
			ACharacter* Character = Cast<ACharacter>(HitResult.GetActor());
			if (Character)
			{
				UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
				if (MovementComponent)
				{
					MovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
					if (!IsTopLocation(Character))
					{
						MovementComponent->AddInputVector(FVector::ZAxisVector);
					}
					else
					{
						MovementComponent->Velocity.Z = 0.0f;
					}
				}
			}
		}
	}
}

bool ATornado::IsTopLocation(AActor* Actor)
{
	return Actor->GetActorLocation().Z >= TopLocation.Z - TopOffset;
}

void ATornado::BeginBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character)
	{
		UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
		if (MovementComponent)
		{
			MovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
		}
	}
}

void ATornado::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeTime);
	TopLocation = GetActorLocation() + BoxTraceHalfSize.Z * 2;
	FTimerHandle DOTTimer;
	SetMultiStageHit(GetOwnerATK() * DamageCoefficient, MSHDelay, EFourElement::EPE_Aqua);
}

void ATornado::EndBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character)
	{
		UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
		if (MovementComponent)
		{
			MovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
			if (IsTopLocation(Character))
			{
				MovementComponent->AddForce(UpForce * 10000000.0f * FVector::ZAxisVector);
			}
		}
	}
}
