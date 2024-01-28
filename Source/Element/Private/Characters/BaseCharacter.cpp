#include "Characters/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Element/DebugMacro.h"
#include "Components/AttributeComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

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
}

FVector ABaseCharacter::GetUnderCharacterFeetLocation()
{
	return GetCapsuleComponent()->GetComponentLocation() - FVector::ZAxisVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
	}
	return DamageAmount;
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
}

void ABaseCharacter::HealCharacter(float Value)
{
	if (Attributes)
	{
		Attributes->RecoveryHP(Value);
	}
}

void ABaseCharacter::HealOverTimeCharacter(float Value, int Count, float Delay)
{
	if (Count == 0) return;
	HealCharacter(Value);

	FTimerHandle HealHandler;
	FTimerDelegate HOTDelegate;
	HOTDelegate.BindUFunction(this, FName("HealOverTimeCharacter"), Value, Count - 1, Delay);
	GetWorldTimerManager().SetTimer(HealHandler, HOTDelegate, Delay, false);
}
