#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/CharacterTypes.h"
#include "PlayerOverlay.generated.h"

class UHorizontalBox;
class UInteractionWidget;

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
	void SetElementSlots(const TArray<EFourElement>& ElementsArray,
		const TArray<EFourElement>& ElementsReadyArray,
		const TArray<EFourElement>& ElementsSelectedArray
	);
	void ShowInteractionWidget(bool WidgetShowed, FString InteractionHint);


private:
	void SetElementSlots(const TArray<EFourElement>& ElementsArray, UHorizontalBox* Slots);

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* ElementSlots;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* ReadySlots;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* SelectedSlots;

	UPROPERTY(meta = (BindWidget))
	UInteractionWidget* InteractionWidget;
};
