// Copyright AFstz.


#include "AbilitySystem/GameplayAbility/MageGameplayAbility.h"

#include "AbilitySystem/MageAttributeSet.h"

FString UMageGameplayAbility::GetDescription(int32 AbilityLevel)
{
	return FString::Printf(TEXT("<Default>技能等级:</> <Level>%d</>"), AbilityLevel);
}

FString UMageGameplayAbility::GetNextLevelDescription(int32 AbilityLevel)
{
	return FString::Printf(TEXT("<Default>技能等级:</> <Level>%d</>"), AbilityLevel);
}

FString UMageGameplayAbility::GetLockedDescription(int32 LevelRequirement)
{
	return FString::Printf(TEXT("<Default>技能需要达到人物等级</> <Level>[%d]</> <Default>级,花费技能点解锁。</>"), LevelRequirement);
}

float UMageGameplayAbility::GetManaCost(int32 InAbilityLevel) const
{
	float ManaCost = 0.f;
	if (UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo ModifierInfo : CostEffect->Modifiers)
		{
			if (ModifierInfo.Attribute == UMageAttributeSet::GetManaAttribute()) // 获取ManaAttribute相关修改
			{
				ModifierInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(InAbilityLevel, ManaCost);
				break;
			}
		}
	}
	return ManaCost * -1;
}

float UMageGameplayAbility::GetCooldownTime(int32 InAbilityLevel) const
{
	float CooldownTime = 0.f;
	if (UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InAbilityLevel, CooldownTime);
	}
	return CooldownTime;
}