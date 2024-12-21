// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/MageWidgetController.h"
#include "SkillMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MAGE_API USkillMenuWidgetController : public UMageWidgetController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature OnSkillPointChangedDelegate;
	
	virtual void BroadcastInitialValue() override;
	virtual void BindCallbacksToDependencies() override;
};
