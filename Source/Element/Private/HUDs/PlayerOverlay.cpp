#include "HUDs/PlayerOverlay.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Components/HorizontalBox.h"

#include "Element/DebugMacro.h"
#include "HUDs/ElementSlotWidget.h"

void UPlayerOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	InitSlotArrays();
}

void UPlayerOverlay::InitSlotArrays()
{
}

void UPlayerOverlay::SetElementSlots(const TArray<EPlayerElement>& ElementsArray,
	const TArray<EPlayerElement>& ElementsReadyArray,
	const TArray<EPlayerElement>& ElementsSelectedArray)
{
	SetElementSlots(ElementsArray, ElementSlots);
	SetElementSlots(ElementsReadyArray, ReadySlots);
	SetElementSlots(ElementsSelectedArray, SelectedSlots);
}

void UPlayerOverlay::SetElementSlots(const TArray<EPlayerElement>& ElementsArray, UHorizontalBox* Slots)
{
	int i;
	const int n = ElementsArray.Num(), m = Slots->GetChildrenCount();
	if (n != m)
	{
		SCREEN_LOG_SINGLE_FRAME(TEXT("PlayerOverlay: ElementsArray Slots Num Not Equal"));
		return;
	}
	for (i = 0; i < n; ++i)
	{
		UElementSlotWidget* ElementSlotWidget = Cast<UElementSlotWidget>(Slots->GetChildAt(i));
		if (ElementSlotWidget)
		{
			ElementSlotWidget->SetElementSlot(ElementsArray[i]);
		}
	}
}
