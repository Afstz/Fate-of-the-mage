// Copyright AFstz.


#include "UI/HUD/MageHUD.h"
#include "UI/Widget/MageUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/SkillMenuWidgetController.h"


UOverlayWidgetController* AMageHUD::GetOverlayWidgetController(const FWidgetControllerParams& InWidgetControllerParams)
{
	if (OverlayWidgetController == nullptr) // 单例,没有则创建
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(InWidgetControllerParams);
		OverlayWidgetController->BindCallbacksToDependencies(); // 向数据层绑定回调函数传递给UI层
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AMageHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& InWidgetControllerParams)
{
	if (AttributeMenuWidgetController == nullptr) // 单例,没有则创建
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(InWidgetControllerParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies(); // 向数据层绑定回调函数传递给UI层
	}
	return AttributeMenuWidgetController;
}

USkillMenuWidgetController* AMageHUD::GetSkillMenuWidgetController(const FWidgetControllerParams& InWidgetControllerParams)
{
	if (SkillMenuWidgetController == nullptr) // 单例,没有则创建
    {
    	SkillMenuWidgetController = NewObject<USkillMenuWidgetController>(this, SkillMenuWidgetControllerClass);
    	SkillMenuWidgetController->SetWidgetControllerParams(InWidgetControllerParams);
    	SkillMenuWidgetController->BindCallbacksToDependencies(); // 向数据层绑定回调函数传递给UI层
    }
    return SkillMenuWidgetController;
}

void AMageHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("No Set OverlayWidgetClass in Blueprint")); // 会在UE崩溃上显示文本
	checkf(OverlayWidgetControllerClass, TEXT("No Set OverlayWidgetControllerClass in Blueprint"));

	OverlayWidget = CreateWidget<UMageUserWidget>(GetWorld(), OverlayWidgetClass);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	OverlayWidgetController = GetOverlayWidgetController(WidgetControllerParams);
	OverlayWidget->SetWidgetController(OverlayWidgetController); // 通过递归绑定回调函数到Controller层的委托
	OverlayWidgetController->BroadcastInitialValue(); // 广播初始化HUD数据
	OverlayWidget->AddToViewport();
}

