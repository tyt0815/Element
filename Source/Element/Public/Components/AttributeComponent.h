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
	FORCEINLINE void SetMaxHP(float n) { MaxHP = n; }
	FORCEINLINE void SetMaxMP(float n) { MaxMP = n; }
	FORCEINLINE void SetHP(float n) {  HP = n; }
	FORCEINLINE void SetMP(float n) {  MP = n; }
	void ReceiveDamage(float Value);
	void RecoveryHP(float Value);
	void UseMP(float Value);
	void RecoveryMP(float Value);
private:
	UPROPERTY(EditAnywhere, Category = Attribute)
	float MaxHP;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float MaxMP;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float HP;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float MP;
};
