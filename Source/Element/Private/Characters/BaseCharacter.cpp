#include "Characters/BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"

#include "Element/DebugMacro.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	SCREEN_LOG_NONE_KEY(TEXT("GetHit is not implemetated"));
}

FVector ABaseCharacter::VectorClamp(const FVector& Vector, const FVector& MinVector, const FVector& MaxVector)
{
	float CameraX = FMath::Clamp(Vector.X, MinVector.X, MaxVector.X);
	float CameraY = FMath::Clamp(Vector.Y, MinVector.Y, MaxVector.Y);
	float CameraZ = FMath::Clamp(Vector.Z, MinVector.Z, MaxVector.Z);
	return FVector(CameraX, CameraY, CameraZ);
}
