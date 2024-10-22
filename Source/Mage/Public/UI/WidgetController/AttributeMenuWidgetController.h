// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/MageWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FMageAttributeData;
class UAttributeData;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeMenuDataSignatrue, const FMageAttributeData&, AttributeData);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MAGE_API UAttributeMenuWidgetController : public UMageWidgetController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValue() override;
	UFUNCTION(BlueprintCallable)
	virtual void BindCallbacksToDependencies() override;

protected:
	UPROPERTY(BlueprintAssignable)
	FAttributeMenuDataSignatrue AttributeMenuDataDelegate; // AttributeRow绑定
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeData> AttributeData; // 存储着对应属性的信息，需要填充值
private:
	void BroadcastAttributeData(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
