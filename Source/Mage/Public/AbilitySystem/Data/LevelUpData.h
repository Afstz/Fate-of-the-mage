// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpData.generated.h"

USTRUCT(BlueprintType)
struct FMageLevelUpData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 LevelUpRequirement = 0; // 所需升级的经验值
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 AttributePointReward = 4; // 属性点奖励
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 SkillPointReward = 1; // 技能点奖励
};

/**
 * 人物等级升级相关数据
 */
UCLASS()
class MAGE_API ULevelUpData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FMageLevelUpData> LevelUpDatas;
	
	int32 FindLevelForXP(int32 XP) const;
};
