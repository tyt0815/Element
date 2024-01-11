#include "Characters/BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"


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

FVector ABaseCharacter::VectorClamp(const FVector& Vector, const FVector& MinVector, const FVector& MaxVector)
{
	float CameraX = FMath::Clamp(Vector.X, MinVector.X, MaxVector.X);
	float CameraY = FMath::Clamp(Vector.Y, MinVector.Y, MaxVector.Y);
	float CameraZ = FMath::Clamp(Vector.Z, MinVector.Z, MaxVector.Z);
	return FVector(CameraX, CameraY, CameraZ);
}
