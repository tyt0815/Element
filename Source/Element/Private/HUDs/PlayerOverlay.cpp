#include "HUDs/PlayerOverlay.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Components/HorizontalBox.h"
#include "Kismet/GameplayStatics.h"

#include "Element/DebugMacro.h"
#include "HUDs/ElementSlotWidget.h"
#include "HUDs/InteractionWidget.h"

void UPlayerOverlay::NativeConstruct()
{
	Super::NativeConstruct();

}

void UPlayerOverlay::SetElementSlots(const TArray<EFourElement>& ElementsArray,
	const TArray<EFourElement>& ElementsReadyArray,
	const TArray<EFourElement>& ElementsSelectedArray)
{
	SetElementSlots(ElementsArray, ElementSlots);
	SetElementSlots(ElementsReadyArray, ReadySlots);
	SetElementSlots(ElementsSelectedArray, SelectedSlots);
}

void UPlayerOverlay::ShowInteractionWidget(bool WidgetShowed, FString InteractionHint)
{
	WidgetShowed ? InteractionWidget->SetVisibility(ESlateVisibility::HitTestInvisible) : InteractionWidget->SetVisibility(ESlateVisibility::Hidden);
	InteractionWidget->SetInteractionHint(InteractionHint);
}

void UPlayerOverlay::SetElementSlots(const TArray<EFourElement>& ElementsArray, UHorizontalBox* Slots)
{
	const int n = ElementsArray.Num(), m = Slots->GetChildrenCount();
	if (n != m)
	{
		SCREEN_LOG_SINGLE_FRAME(TEXT("PlayerOverlay: ElementsArray Slots Num Not Equal"));
		return;
	}
	for (int i = 0; i < n; ++i)
	{
		UElementSlotWidget* ElementSlotWidget = Cast<UElementSlotWidget>(Slots->GetChildAt(i));
		if (ElementSlotWidget)
		{
			ElementSlotWidget->SetElementSlot(ElementsArray[i]);
		}
	}
}