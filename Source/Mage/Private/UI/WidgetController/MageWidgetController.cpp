// Copyright AFstz.


#include "UI/WidgetController/MageWidgetController.h"

void UMageWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& InWidgetControllerParams)
{
	PlayerController = InWidgetControllerParams.PlayerController;
	PlayerState = InWidgetControllerParams.PlayerState;
	AbilitySystemComponent = InWidgetControllerParams.AbilitySystemComponent;
	AttributeSet = InWidgetControllerParams.AttributeSet;
	
}

void UMageWidgetController::BroadcastInitialValue()
{
}

void UMageWidgetController::BindCallbacksToDependencies()
{
}
