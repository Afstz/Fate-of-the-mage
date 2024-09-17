// Copyright AFstz.


#include "UI/Widget/MageUserWidget.h"

void UMageUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerIsSet();
}