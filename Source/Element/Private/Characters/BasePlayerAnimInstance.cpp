#include "Characters/BasePlayerAnimInstance.h"
#include "Characters/BasePlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBasePlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<ABasePlayer>(TryGetPawnOwner());
	if (PlayerCharacter != nullptr)
	{
		PlayerMovement = PlayerCharacter->GetCharacterMovement();
	}
}

void UBasePlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (PlayerMovement != nullptr)
	{
		Velocity = PlayerMovement->Velocity;
		GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);
		ShouldMove = IsMove();
		IsFalling = PlayerMovement->IsFalling();
	}
}

bool UBasePlayerAnimInstance::IsMove()
{
	if (PlayerMovement == nullptr)
	{
		return false;
	}
	return GroundSpeed > 3.f && !PlayerMovement->GetCurrentAcceleration().Equals(FVector::ZeroVector, 0);
}
