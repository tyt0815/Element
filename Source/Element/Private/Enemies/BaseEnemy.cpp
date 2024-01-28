#include "Enemies/BaseEnemy.h"

#include "Element/DebugMacro.h"
#include "Components/AttributeComponent.h"

ABaseEnemy::ABaseEnemy()
{
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
}

float ABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	return DamageAmount;
}

void ABaseEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
}
