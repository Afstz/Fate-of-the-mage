// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MageWidgetController.generated.h"

class UAbilityData;
class UMageAttributeSet;
class UMageAbilitySystemComponent;
class AMagePlayerState;
class AMagePlayerController;
class UAttributeSet;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerLevelChangedSignature, int32, NewValue, bool, bUpgraded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityDataSignature, const FMageAbilityData& , Data);

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()
	FWidgetControllerParams(){}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
		: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

/**
 * 
 */
UCLASS()
class MAGE_API UMageWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& InWidgetControllerParams);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValue();
	UFUNCTION(BlueprintCallable)
	virtual void BindCallbacksToDependencies();

	UPROPERTY(BlueprintAssignable, Category = "GAS | WidgetData")
	FAbilityDataSignature AbilityDataDelegate;
protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<AMagePlayerController> MagePlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<AMagePlayerState> MagePlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UMageAbilitySystemComponent> MageAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UMageAttributeSet> MageAttributeSet;

	AMagePlayerController* GetMagePC();
	AMagePlayerState* GetMagePS();
	UMageAbilitySystemComponent* GetMageASC();
	UMageAttributeSet* GetMageAS();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WidgetData")
	TObjectPtr<UAbilityData> AbilityData; // 技能配置信息
	void BroadcastAbilityData();

	bool GetAbilityDescFromAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription); // 获取技能描述

	UFUNCTION(BlueprintCallable)
	virtual void ClearAllDelegate(); // 清理委托所有回调函数
};
