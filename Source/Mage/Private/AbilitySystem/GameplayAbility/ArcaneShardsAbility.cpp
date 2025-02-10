// Copyright AFstz.


#include "AbilitySystem/GameplayAbility/ArcaneShardsAbility.h"

FString UArcaneShardsAbility::GetDescription(int32 AbilityLevel)
{
	float CausedDamage = GetAbilityDamage(AbilityLevel);
	float ManaCost = GetManaCost(AbilityLevel);
	float CooldownTime = GetCooldownTime(AbilityLevel);
	int32 InMaxNumShards = GetCurrentShardsNum(AbilityLevel);
	
	return FString::Printf(TEXT(
		// Title
		"<Title>奥术碎片</>\n\n"
		"<Default>技能等级:</><Level>%d</>\n"
		"<Default>伤害:</><Damage>%.0f</>\n"
		"<Default>耗蓝量:</><ManaCost>%.1f</>\n"
		"<Default>冷却时间:</><CooldownTime>%.1f</>\n\n"
		// Description
		"<Small>指定位置生成 %d 个碎片，击中敌人造成击退。</>")
		
		, AbilityLevel, CausedDamage, ManaCost, CooldownTime, InMaxNumShards);
}

FString UArcaneShardsAbility::GetNextLevelDescription(int32 AbilityLevel)
{
	float CausedDamage = GetAbilityDamage(AbilityLevel);
	float ManaCost = GetManaCost(AbilityLevel);
	float CooldownTime = GetCooldownTime(AbilityLevel);
	int32 InMaxNumShards = GetCurrentShardsNum(AbilityLevel);
	
	return FString::Printf(TEXT(
		// Title
		"<Title>下一个等级</>\n\n"
		"<Default>技能等级:</><Level>%d</>\n"
		"<Default>伤害:</><Damage>%.0f</>\n"
		"<Default>耗蓝量:</><ManaCost>%.1f</>\n"
		"<Default>冷却时间:</><CooldownTime>%.1f</>\n\n"
		// Description
		"<Small>指定位置生成 %d 个碎片，击中敌人造成击退，升级造成更高的伤害。</>")
		
		, AbilityLevel, CausedDamage, ManaCost, CooldownTime, InMaxNumShards);
}

int32 UArcaneShardsAbility::GetCurrentShardsNum(int32 InAbilityLevel)
{
	return FMath::Min(MaxNumShards, InAbilityLevel);
}