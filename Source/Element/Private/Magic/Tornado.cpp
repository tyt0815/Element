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
				Character->GetCharacterMovement()->Velocity.Z = UpSpeed;
			}
		}
	}
}

void ATornado::BeginBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MakeCharacterJump();
}

void ATornado::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeTime);
	FTimerHandle DOTTimer;
	SetMultiStageHit(GetOwnerATK() * DamageCoefficient, MSHDelay, EFourElement::EPE_Aqua);

	MakeCharacterJump();
}

void ATornado::EndBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character)
	{
		UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
		if (MovementComponent)
		{
			MovementComponent->AddForce(PushUpForce * FVector::ZAxisVector);
		}
	}
}

void ATornado::MakeCharacterJump()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> Ignores;
	BoxTraceMulti(HitResults, Ignores);

	for (auto& HitResult : HitResults)
	{
		ACharacter* Character = Cast<ACharacter>(HitResult.GetActor());
		if (Character && Character->GetCharacterMovement()->Velocity.Z == 0.0f)
		{
			Character->GetCharacterMovement()->AddForce(10000000.0f * FVector::ZAxisVector);
		}
	}
}
