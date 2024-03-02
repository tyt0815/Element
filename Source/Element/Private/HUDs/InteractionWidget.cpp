#include "HUDs/InteractionWidget.h"
#include "Components/TextBlock.h"

void UInteractionWidget::SetInteractionHint(FString Hint)
{
	InteractionHint->SetText(FText::FromString(Hint));
}
