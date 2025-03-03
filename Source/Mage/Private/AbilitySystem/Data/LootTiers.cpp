// Copyright AFstz.


#include "AbilitySystem/Data/LootTiers.h"

TArray<FLootData> ULootTiers::GetSpawnLootDatas()
{
	TArray<FLootData> ReturnLootDatas;
	
	for (FLootData LootData : LootDatas)
	{
		// 生成这个战利品最大的数量
		for (int32 i = 0; i < LootData.MaxSpawnNums; ++i)
		{
			// 计算生成几率
			if (FMath::RandRange(1, 100) <= LootData.ChanceToSpawn)
			{
				ReturnLootDatas.Add(LootData);
			}
		}
	}
	return ReturnLootDatas;
}