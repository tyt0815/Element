#include "Magic/Tornado.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Element/DebugMacro.h"

void ATornado::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InitActorsToIgnore();
	while (true)
	{
		FHitResult HitResult;
		BoxTrace(HitResult);
		if (HitResult.GetActor() == nullptr) break;
		if (HitResult.GetActor() == GetOwner())
		{
			ACharacter* Character = Cast<ACharacter>(HitResult.GetActor());
			if (Character)
			{
				if (!IsTopLocation(Character))
				{
					if (Character->GetCharacterMovement()->Velocity.Z < 1)
						Character->GetCharacterMovement()->AddForce(FVector::ZAxisVector * 100000.0f);
					Character->GetCharacterMovement()->Velocity.Z = UpSpeed;
				}
				else
					Character->GetCharacterMovement()->Velocity.Z = TopSpeed;
			}
		}
	}
}

bool ATornado::IsTopLocation(AActor* Actor)
{
	return Actor->GetActorLocation().Z >= TopLocation.Z - TopOffset;
}

void ATornado::BeginPlay()
{
	Super::BeginPlay();

	EndMagicAfter(LifeTime);
	TopLocation = GetActorLocation() + BoxTraceHalfSize.Z * 2;
	FTimerHandle DOTTimer;
	SetMultiStageHit(GetOwnerATK() * DamageCoefficient, MSHDelay);
}

void ATornado::InitActorsToIgnore()
{
	ActorsToIgnore.Empty();
}

void ATornado::EndBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character && IsTopLocation(Character))
	{
		Character->GetCharacterMovement()->AddForce(UpForce * 10000000.0f * FVector::ZAxisVector);
	}
}
