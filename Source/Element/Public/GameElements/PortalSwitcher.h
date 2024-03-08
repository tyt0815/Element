#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ReactToTriggerInterface.h"
#include "PortalSwitcher.generated.h"

class APortal;

UCLASS()
class ELEMENT_API APortalSwitcher : public AActor, public IReactToTriggerInterface
{
	GENERATED_BODY()
	
public:	
	APortalSwitcher();

protected:
	virtual void ReactToTrigger_Implementation(AActor* Trigger) override;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Attributes)
	TArray<APortal*> SwitchedPortals;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Attributes)
	TArray<AActor*> Switchers;
private:
};
