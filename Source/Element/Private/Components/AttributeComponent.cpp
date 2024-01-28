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

void UAttributeComponent::ReceiveDamage(float Value)
{
	HP = FMath::Clamp(HP - Value, 0.0f, MaxHP);
	if (GetOwner())
	{
		SCREEN_LOG_NONE_KEY(GetOwner()->GetName() + FString("'s HP: ") + FString::SanitizeFloat(HP));
	}
}

void UAttributeComponent::RecoveryHP(float Value)
{
	ReceiveDamage(-Value);
}

void UAttributeComponent::UseMP(float Value)
{
	MP = FMath::Clamp(MP - Value, 0.0f, MaxMP);
}

void UAttributeComponent::RecoveryMP(float Value)
{
	UseMP(-Value);
}

