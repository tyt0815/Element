#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Door.generated.h"



UCLASS()
class ELEMENT_API ADoor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:
	ADoor();
	virtual FString GetInteractionHint_Implementation() const { return Opened ? FString("Close") : FString("Open"); }

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Open();
	virtual void Open_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Close();
	virtual void Close_Implementation();

	UFUNCTION(BlueprintPure)
	float GetOpenDirection(AActor* OpeningActor) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* RootMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* DoorInnerMesh;

private:
	bool Opened;
};
