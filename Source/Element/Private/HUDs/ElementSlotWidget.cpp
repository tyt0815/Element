#include "HUDs/ElementSlotWidget.h"
#include "Components/WidgetSwitcher.h"

void UElementSlotWidget::SetElementSlot(EFourElement Element)
{
	int ElementIndex;
	switch (Element)
	{
	case EFourElement::EPE_Ignis:
		ElementIndex = 1;
		break;
	case EFourElement::EPE_Aqua:
		ElementIndex = 2;
		break;
	case EFourElement::EPE_Ventus:
		ElementIndex = 3;
		break;
	case EFourElement::EPE_Terra:
		ElementIndex = 4;
		break;
	default:
		ElementIndex = 0;
		break;
	}
	if (ElementSwitcher)
	{
		ElementSwitcher->SetActiveWidgetIndex(ElementIndex);
	}
}
