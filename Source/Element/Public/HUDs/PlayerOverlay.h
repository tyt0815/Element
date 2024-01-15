#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/CharacterTypes.h"
#include "PlayerOverlay.generated.h"

class UHorizontalBox;

UCLASS()
class ELEMENT_API UPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	/*
	* Elements
	*/
public:
	void InitSlotArrays();
	void SetElementSlots(const TArray<EPlayerElement>& ElementsArray,
		const TArray<EPlayerElement>& ElementsReadyArray,
		const TArray<int8>& ElementsSelectedArray
	);

private:
	void SetElementSlots(const TArray<EPlayerElement>& ElementsArray, UHorizontalBox* Slots);
	void SetSelectedSlot(const TArray<EPlayerElement>& ElementsArray, const TArray<int8>& ElementsSelectedArray);

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* ElementSlots;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* ReadySlots;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* SelectedSlots;
};
