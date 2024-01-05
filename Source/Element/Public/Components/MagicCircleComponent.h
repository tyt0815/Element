#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MagicCircleComponent.generated.h"

class UNiagaraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ELEMENT_API UMagicCircleComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UMagicCircleComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere);
	UNiagaraComponent* MagicBulletCircle;

protected:
	virtual void BeginPlay() override;
};
