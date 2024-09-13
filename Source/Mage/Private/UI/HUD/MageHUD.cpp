// Copyright AFstz.


#include "UI/HUD/MageHUD.h"
#include "UI/Widget/MageUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"


UOverlayWidgetController* AMageHUD::GetOverlayWidgetController(const FWidgetControllerParams& InWigetControllerParams)
{
	if (OverlayWidgetController == nullptr) // 单例,没有则创建
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(InWigetControllerParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

void AMageHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("No Set OverlayWidgetClass in Blueprint")); // 会在UE崩溃上显示文本
	checkf(OverlayWidgetControllerClass, TEXT("No Set OverlayWidgetControllerClass in Blueprint"));

	OverlayWidget = CreateWidget<UMageUserWidget>(GetWorld(), OverlayWidgetClass);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	OverlayWidgetController = GetOverlayWidgetController(WidgetControllerParams);
	OverlayWidget->SetWidgetController(OverlayWidgetController); // 通过递归绑定回调函数到Controller层委托
	OverlayWidgetController->BroadcastInitialValue(); // 广播初始化HUD数据
	OverlayWidget->AddToViewport();
}

