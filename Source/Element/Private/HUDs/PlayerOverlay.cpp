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
	const TArray<int8>& ElementsSelectedArray)
{
	SetElementSlots(ElementsArray, ElementSlots);
	SetElementSlots(ElementsReadyArray, ReadySlots);
	SetSelectedSlot(ElementsArray, ElementsSelectedArray);
}

void UPlayerOverlay::SetElementSlots(const TArray<EPlayerElement>& ElementsArray, UHorizontalBox* Slots)
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

void UPlayerOverlay::SetSelectedSlot(const TArray<EPlayerElement>& ElementsArray, const TArray<int8>& ElementsSelectedArray)
{
	const int n = ElementsSelectedArray.Num(), m = SelectedSlots->GetChildrenCount();
	if (n != m)
	{
		SCREEN_LOG_SINGLE_FRAME(TEXT("PlayerOverlay: ElementsSelected Array Slots Num Not Equal"));
		return;
	}

	int i;
	for (i = 0; i < ElementsSelectedArray.Num(); ++i)
	{
		if (ElementsSelectedArray[i] < 0) break;
		UElementSlotWidget* ElementSlotWidget = Cast<UElementSlotWidget>(SelectedSlots->GetChildAt(i));
		if (ElementSlotWidget)
		{
			ElementSlotWidget->SetElementSlot(ElementsArray[ElementsSelectedArray[i]]);
		}
	}
	for (; i < ElementsSelectedArray.Num(); ++i)
	{
		UElementSlotWidget* ElementSlotWidget = Cast<UElementSlotWidget>(SelectedSlots->GetChildAt(i));
		if (ElementSlotWidget)
		{
			ElementSlotWidget->SetElementSlot(EPlayerElement::EPE_None);
		}
	}
}
