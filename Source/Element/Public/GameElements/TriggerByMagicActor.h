#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "Interfaces/TriggerInterface.h"
#include "TriggerByMagicActor.generated.h"

class UBoxComponent;

UCLASS()
class ELEMENT_API ATriggerByMagicActor : public AActor, public IHitInterface, public ITriggerInterface
{
	GENERATED_BODY()
	
public:	
	ATriggerByMagicActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter, EFourElement Element) override;
	virtual void Trigger_Implementation(bool Condition, AActor* TriggeringActor, TArray<AActor*>& TriggerTargets) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* TriggerBox;

	UPROPERTY(EditInstanceOnly, Category = Attributes)
	TArray<AActor*> Targets;

	UPROPERTY(EditAnywhere, Category = Attributes)
	TEnumAsByte<EFourElement> TriggerElement;

	/*Duration > 0 인 경우, Duration초 이후에 Trigger가 Off됨*/
	UPROPERTY(EditInstanceOnly, Category = Attributes)
	float Duration = 0.0f;


private:
	FTimerHandle FixedTermHandle;
	

	UFUNCTION()
	void HaltTrigger(AActor* TriggeringActor, TArray<AActor*>& TriggerTargets);
};
