#include "Characters/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Element/DebugMacro.h"
#include "Components/AttributeComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Block);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Block);
	GetCharacterMovement()->AirControl = 1.0f;

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

float ABaseCharacter::GetATK()
{
	float ATK = 0;
	ATK += Attributes ? Attributes->GetATK() : 0;
	return ATK;
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