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
	virtual void BroadcastInitialValue() override;
	virtual void BindCallbacksToDependencies() override;

protected:
	UPROPERTY(BlueprintAssignable)
	FAttributeMenuDataSignatrue AttributeMenuDataDelegate; // AttributeRow绑定
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeData> AttributeData; // 存储着对应属性的信息

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature OnAttributePointChangedDelegate;

	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(const FGameplayTag& AttributeTag); // 根据标签提升对应属性
private:
	void BroadcastAttributeData(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
