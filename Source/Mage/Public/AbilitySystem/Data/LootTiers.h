// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootTiers.generated.h"

class AMageEffectActor;

USTRUCT(BlueprintType)
struct FLootData
{
	GENERATED_BODY()

	// 战利品类
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<AActor> LootClass;
	// 生成几率
	UPROPERTY(EditAnywhere)
	int32 ChanceToSpawn = 0;
	// 最大生成数量
	UPROPERTY(EditAnywhere)
	int32 MaxSpawnNums = 0;
	// 是否根据角色等级覆盖生成物品的等级(默认1级)
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bOverrideLootLevel = true;
};

/**
 *  战利品数据
 */
UCLASS()
class MAGE_API ULootTiers : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FLootData> LootDatas;

	UFUNCTION(BlueprintCallable)
	TArray<FLootData> GetSpawnLootDatas();
};
