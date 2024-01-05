#include "Components/MagicCircleComponent.h"
#include "NiagaraComponent.h"

UMagicCircleComponent::UMagicCircleComponent() : USceneComponent()
{

}


void UMagicCircleComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UMagicCircleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

