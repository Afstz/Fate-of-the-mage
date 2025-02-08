// Copyright AFstz.


#include "AbilitySystem/GameplayAbility/LightningChainAbility.h"

FString ULightningChainAbility::GetDescription(int32 AbilityLevel)
{
	float CausedDamage = GetAbilityDamage(AbilityLevel);
	float ManaCost = GetManaCost(AbilityLevel);
	float CooldownTime = GetCooldownTime(AbilityLevel);
	int32 InNumMaxShockTargets = FMath::Min(NumMaxShockTargets, AbilityLevel);
	
	return FString::Printf(TEXT(
		// Title
		"<Title>闪电链</>\n\n"
		"<Default>技能等级:</><Level>%d</>\n"
		"<Default>伤害:</><Damage>%.0f</>\n"
		"<Default>耗蓝量:</><ManaCost>%.1f</>\n"
		"<Default>冷却时间:</><CooldownTime>%.1f</>\n\n"
		// Description
		"<Small>生成 %d 个闪电链，闪电链会从第一个目标扩散，击中敌人几率造成眩晕。</>")
		
		, AbilityLevel, CausedDamage, ManaCost, CooldownTime, InNumMaxShockTargets);
}

FString ULightningChainAbility::GetNextLevelDescription(int32 AbilityLevel)
{
	float CausedDamage = GetAbilityDamage(AbilityLevel);
	float ManaCost = GetManaCost(AbilityLevel);
	float CooldownTime = GetCooldownTime(AbilityLevel);
	int32 InNumMaxShockTargets = FMath::Min(NumMaxShockTargets, AbilityLevel);
	
	return FString::Printf(TEXT(
		// Title
		"<Title>下一个等级</>\n\n"
		"<Default>技能等级:</><Level>%d</>\n"
		"<Default>伤害:</><Damage>%.0f</>\n"
		"<Default>耗蓝量:</><ManaCost>%.1f</>\n"
		"<Default>冷却时间:</><CooldownTime>%.1f</>\n\n"
		// Description
		"<Small>生成 %d 个闪电链，闪电链会从第一个目标扩散，击中敌人几率造成眩晕，造成更高的伤害。</>")
		
		, AbilityLevel, CausedDamage, ManaCost, CooldownTime, InNumMaxShockTargets);	
}