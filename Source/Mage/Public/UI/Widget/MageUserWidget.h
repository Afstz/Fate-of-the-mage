// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MageUserWidget.generated.h"

/**
 *  游戏主场景所有Widget的基类
 */
UCLASS()
class MAGE_API UMageUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable) 
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly) 
	TObjectPtr<UObject> WidgetController;
	
protected:
	UFUNCTION(BlueprintImplementableEvent) // C++声明,蓝图实现
	void WidgetControllerIsSet();
};
