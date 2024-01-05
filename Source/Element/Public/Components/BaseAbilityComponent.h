#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseAbilityComponent.generated.h"

class UInputAction;
class UEnhancedInputComponent;

struct FInputActionInstance;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ELEMENT_API UBaseAbilityComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UBaseAbilityComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnRegister() override;
	virtual void OnUnregister() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Component | Input")
	UInputAction* InputAction;

	UFUNCTION(BlueprintNativeEvent)
	void OnStarted(const FInputActionInstance& InputInstance);
	virtual void OnStarted_Implementation(const FInputActionInstance& InputInstance);

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability Component | Input")
	void AbilityTick(const FInputActionInstance& InputInstance);
	virtual void AbilityTick_Implementation(const FInputActionInstance& InputInstance);

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability Component | Input")
	void OnCancelled(const FInputActionInstance& InputInstance);
	virtual void OnCancelled_Implementation(const FInputActionInstance& InputInstance);

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability Component | Input")
	void ActivateAbility(const FInputActionInstance& InputInstance);
	virtual void ActivateAbility_Implementation(const FInputActionInstance& InputInstance);

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability Component | Input")
	void OnCompleted(const FInputActionInstance& InputInstance);
	virtual void OnCompleted_Implementation(const FInputActionInstance& InputInstance);

protected:
	virtual void BeginPlay() override;

private:
	uint32 ActionStartedBindingHandle;
	uint32 ActionOngoingBindingHandle;
	uint32 ActionCanceledBindingHandle;
	uint32 ActionTriggeredBindingHandle;
	uint32 ActionCompletedBindingHandle;

	ACharacter* OwningCharacter;
	UEnhancedInputComponent* EnhancedInputComponent;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability Component")
	ACharacter* GetOwningCharacter();
};
