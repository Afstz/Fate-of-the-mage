// Copyright AFstz.


#include "AbilitySystem/GameplayAbility/FireBallAbility.h"

FString UFireBallAbility::GetDescription(int32 AbilityLevel)
{
	float CausedDamage = GetAbilityDamage(AbilityLevel);
	float ManaCost = GetManaCost(AbilityLevel);
	float CooldownTime = GetCooldownTime(AbilityLevel);
	
	return FString::Printf(TEXT(
		// Title
		"<Title>火球术</>\n\n"
		"<Default>技能等级:</><Level>%d</>\n"
		"<Default>伤害:</><Damage>%.0f</>\n"
		"<Default>耗蓝量:</><ManaCost>%.1f</>\n"
		"<Default>冷却时间:</><CooldownTime>%.1f</>\n\n"
		// Description
		"<Small>发射 %d 个火球，击中敌人发生爆炸。</>")
		
		, AbilityLevel, CausedDamage, ManaCost, CooldownTime, AbilityLevel);

}

FString UFireBallAbility::GetNextLevelDescription(int32 AbilityLevel)
{
	float CausedDamage = GetAbilityDamage(AbilityLevel);
	float ManaCost = GetManaCost(AbilityLevel);
	float CooldownTime = GetCooldownTime(AbilityLevel);
	
	return FString::Printf(TEXT(
		// Title
		"<Title>下一个等级</>\n\n"
		"<Default>技能等级:</><Level>%d</>\n"
		"<Default>伤害:</><Damage>%.0f</>\n"
		"<Default>耗蓝量:</><ManaCost>%.1f</>\n"
		"<Default>冷却时间:</><CooldownTime>%.1f</>\n\n"
		// Description
		"<Small>发射 %d 个火球，击中敌人发生爆炸。造成更高的伤害。</>")
		
		, AbilityLevel, CausedDamage, ManaCost, CooldownTime, AbilityLevel);
}