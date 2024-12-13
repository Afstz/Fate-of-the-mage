// Copyright AFstz.


#include "AbilitySystem/Data/LevelUpData.h"

int32 ULevelUpData::FindLevelForXP(int32 XP) const
{
	int32 Level = 1;
	
	// LevelUpDatas[0]是占位符;
	
	for (int i = 1; i < LevelUpDatas.Num(); i++)
	{
		if (XP >= LevelUpDatas[i].LevelUpRequirement)
		{
			Level++; // 满足升级条件
		}
		else
		{
			break; // 不满足退出循环
		}
	}
	
	return Level;
}