#include "HUDs/ElementSlotWidget.h"
#include "Components/WidgetSwitcher.h"

void UElementSlotWidget::SetElementSlot(EPlayerElement Element)
{
	int ElementIndex;
	switch (Element)
	{
	case EPlayerElement::EPE_Ignis:
		ElementIndex = 1;
		break;
	case EPlayerElement::EPE_Aqua:
		ElementIndex = 2;
		break;
	case EPlayerElement::EPE_Ventus:
		ElementIndex = 3;
		break;
	case EPlayerElement::EPE_Terra:
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
