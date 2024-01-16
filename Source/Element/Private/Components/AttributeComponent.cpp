#include "Components/AttributeComponent.h"

#include "Element/DebugMacro.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
		
}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	HP = FMath::Clamp(HP - Damage, 0.0f, MaxHP);
	SCREEN_LOG(0, FString::SanitizeFloat(HP));
}

void UAttributeComponent::UseMP(float MPCost)
{
	MP = FMath::Clamp(MP - MPCost, 0.0f, MaxMP);
}

