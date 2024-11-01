// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "MageAbilitySystemGlobals.generated.h"

/**
 *  自定义的AbilitySystemGlobals类,保存能力系统的全局数据
 *  
 *  需要在Config/DefaultGame.ini文件里进行配置
 */
UCLASS()
class MAGE_API UMageAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	// 用于在MakeEffectContext时返回自定义的上下文信息
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override; 
	
};
