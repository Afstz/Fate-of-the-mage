// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitCooldownChange.generated.h"


struct FActiveGameplayEffectHandle;
struct FGameplayEffectSpec;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);

/**
 *  用于获取技能冷却时间的异步蓝图结点
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class MAGE_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UWaitCooldownChange* WaitCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag);

	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownStart; // 监听冷却时间开始
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownEnd; // 监听冷却时间结束
	
	UFUNCTION(BlueprintCallable)
	void EndTask(); // 释放内存
protected:
	void OnCooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);
	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTag CooldownTag; // 要监听的冷却标签
};
