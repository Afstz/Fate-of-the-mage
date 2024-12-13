// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/MageWidgetController.h"
#include "OverlayWidgetController.generated.h"

struct FMageAbilityData;
class UMageAbilitySystemComponent;
class UAbilityData;
class UMageUserWidget;

USTRUCT(BlueprintType)
struct FMessageWdigetData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Message = FText();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UMageUserWidget> MessageWidget;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* WidgetImage = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetSignature, const FMessageWdigetData&, MessageWdigetData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityDataSignature, const FMageAbilityData& , Data);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MAGE_API UOverlayWidgetController : public UMageWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValue() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChangedSignature OnManaChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS | WidgetData")
	FMessageWidgetSignature MessageWidgetDelegate;
	UPROPERTY(BlueprintAssignable, Category = "GAS | WidgetData")
	FAbilityDataSignature AbilityDataDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS | PlayerStat")
	FOnAttributeChangedSignature OnXPPercentChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category = "GAS | PlayerStat")
	FOnPlayerStatChangedSignature OnLevelChangedDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WidgetData")
	TObjectPtr<UDataTable> MessageDataTable; // 拾取消息配置信息
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WidgetData")
	TObjectPtr<UAbilityData> AbilityData; // 技能配置信息
	
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

	void OnInitializeStartupAbilities(UMageAbilitySystemComponent* MageASC);
	void OnXPChanged(const int32 NewXP) const;
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	T* DataTableRow = DataTable->FindRow<T>(Tag.GetTagName(), TEXT("")); // 根据对应Tag查找表行
	return DataTableRow;
}
