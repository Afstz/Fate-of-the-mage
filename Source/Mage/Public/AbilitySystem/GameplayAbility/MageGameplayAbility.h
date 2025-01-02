// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MageGameplayAbility.generated.h"

/**
 *  自定义的玩法技能基类
 */
UCLASS()
class MAGE_API UMageGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

	virtual FString GetDescription(int32 AbilityLevel = 1.f); // 获取当前等级技能描述
	virtual FString GetNextLevelDescription(int32 AbilityLevel = 1.f); // 获取下一个等级技能描述
	static FString GetLockedDescription(int32 LevelRequirement); // 锁定技能信息
protected:
	float GetManaCost(int32 InAbilityLevel) const; // 获得耗蓝量
	float GetCooldownTime(int32 InAbilityLevel) const; // 获得冷却时间
};
