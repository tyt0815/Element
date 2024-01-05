#include "Components/BaseAbilityComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerInput.h"


UBaseAbilityComponent::UBaseAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UBaseAbilityComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UBaseAbilityComponent::OnRegister()
{
	Super::OnRegister();

	OwningCharacter = Cast<ACharacter>(GetOwner());

	if (OwningCharacter)
	{
		EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(OwningCharacter->InputComponent);
		if (EnhancedInputComponent)
		{
			ActionStartedBindingHandle = EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &UBaseAbilityComponent::OnStarted).GetHandle();
			ActionOngoingBindingHandle = EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Ongoing, this, &UBaseAbilityComponent::AbilityTick).GetHandle();
			ActionCanceledBindingHandle = EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Canceled, this, &UBaseAbilityComponent::OnCancelled).GetHandle();
			ActionTriggeredBindingHandle = EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &UBaseAbilityComponent::ActivateAbility).GetHandle();
			ActionCompletedBindingHandle = EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &UBaseAbilityComponent::OnCompleted).GetHandle();
		}
	}
}

void UBaseAbilityComponent::OnUnregister()
{
	Super::OnUnregister();

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->RemoveActionBindingForHandle(ActionStartedBindingHandle);
		EnhancedInputComponent->RemoveActionBindingForHandle(ActionOngoingBindingHandle);
		EnhancedInputComponent->RemoveActionBindingForHandle(ActionCanceledBindingHandle);
		EnhancedInputComponent->RemoveActionBindingForHandle(ActionTriggeredBindingHandle);
		EnhancedInputComponent->RemoveActionBindingForHandle(ActionCompletedBindingHandle);
	}

	OwningCharacter = nullptr;
	EnhancedInputComponent = nullptr;
}

void UBaseAbilityComponent::OnStarted_Implementation(const FInputActionInstance& InputInstance)
{
}

void UBaseAbilityComponent::AbilityTick_Implementation(const FInputActionInstance& InputInstance)
{
}

void UBaseAbilityComponent::OnCancelled_Implementation(const FInputActionInstance& InputInstance)
{
}

void UBaseAbilityComponent::ActivateAbility_Implementation(const FInputActionInstance& InputInstance)
{
}

void UBaseAbilityComponent::OnCompleted_Implementation(const FInputActionInstance& InputInstance)
{
}

ACharacter* UBaseAbilityComponent::GetOwningCharacter()
{
	return OwningCharacter;
}


void UBaseAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

