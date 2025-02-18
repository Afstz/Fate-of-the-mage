// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadScreenWidget.generated.h"

/**
 *  所有LoadScreenWidget的基类
 */
UCLASS()
class MAGE_API ULoadScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 用于初始化每一个Widget
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintInitializeWidget(); 
};
