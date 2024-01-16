#include "Characters/BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"

#include "Element/DebugMacro.h"
#include "Components/AttributeComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attribute"));
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
	
	Attributes->SetHP(Attributes->GetMaxHP());
	Attributes->SetMP(Attributes->GetMaxMP());
	SCREEN_LOG(3, TEXT("Sibal"));
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
	}

	return DamageAmount;
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	SCREEN_LOG_NONE_KEY(FString("GetHit is not implemetated: ") + GetName());
}

FVector ABaseCharacter::VectorClamp(const FVector& Vector, const FVector& MinVector, const FVector& MaxVector)
{
	float CameraX = FMath::Clamp(Vector.X, MinVector.X, MaxVector.X);
	float CameraY = FMath::Clamp(Vector.Y, MinVector.Y, MaxVector.Y);
	float CameraZ = FMath::Clamp(Vector.Z, MinVector.Z, MaxVector.Z);
	return FVector(CameraX, CameraY, CameraZ);
}
