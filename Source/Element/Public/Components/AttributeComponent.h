#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ELEMENT_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/*
	* Character Attributes
	*/
public:
	FORCEINLINE float GetMaxHP() { return MaxHP; }
	FORCEINLINE float GetMaxMP() { return MaxMP; }
	FORCEINLINE float GetHP() { return HP; }
	FORCEINLINE float GetMP() { return MP; }
	FORCEINLINE float GetATK() { return ATK; }
	FORCEINLINE void SetMaxHP(float Value) { MaxHP = Value; }
	FORCEINLINE void SetMaxMP(float Value) { MaxMP = Value; }
	FORCEINLINE void SetHP(float Value) {  HP = Value; }
	FORCEINLINE void SetMP(float Value) {  MP = Value; }
	FORCEINLINE void SetATK(float Value) { ATK = Value; }
	void ReceiveDamage(float Value);
	void RecoveryHP(float Value);
	void UseMP(float Value);
	void RecoveryMP(float Value);
private:
	UPROPERTY(EditAnywhere, Category = Attributes)
	float MaxHP;

	UPROPERTY(EditAnywhere, Category = Attributes)
	float MaxMP;

	UPROPERTY(EditAnywhere, Category = Attributes)
	float HP;

	UPROPERTY(EditAnywhere, Category = Attributes)
	float MP;

	UPROPERTY(EditAnywhere, Category = Attributes)
	float ATK;
};
