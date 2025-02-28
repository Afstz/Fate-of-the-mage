// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "MageSaveGame.generated.h"

class UGameplayAbility;
class UGameplayEffect;

// 地图中Actor的数据
USTRUCT(BlueprintType)
struct FSavedActor // 对象不能保存指针，因为指针保存在内存里，每次加载都不一致。
{
	GENERATED_BODY()

	UPROPERTY()
	FName ActorName = FName();

	UPROPERTY()
	FTransform ActorTransform = FTransform();

	UPROPERTY()
	TArray<uint8> SerializeBytes; // 用于存储Actor序列化数据的字节数组
};

inline bool operator==(const FSavedActor& Left, const FSavedActor& Right)
{
	return Left.ActorName == Right.ActorName;
}

// 地图中Actor的数据
USTRUCT(BlueprintType)
struct FSavedWorld
{
	GENERATED_BODY()

	UPROPERTY()
	FString MapAssetName = FString();

	// 地图中保存的Actor
	UPROPERTY()
	TArray<FSavedActor> SavedActors;
};

inline bool operator==(const FSavedWorld& Left, const FSavedWorld& Right)
{
	return Left.MapAssetName == Right.MapAssetName;
}

// 保存的技能数据
USTRUCT(BlueprintType)
struct FSavedAbilityData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTag AbilityTag = FGameplayTag(); // 技能标签

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 AbilityLevel = 1; // 技能等级
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag = FGameplayTag(); // 技能状态标签

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTag AbilityType = FGameplayTag(); // 技能类型标签 (主动/被动)

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag AbilityInputTag = FGameplayTag(); // 技能输入标签

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> AbilityClass; // 技能类
};

inline bool operator==(const FSavedAbilityData& Left, const FSavedAbilityData& Right)
{
	return Left.AbilityTag.MatchesTagExact(Right.AbilityTag);
}

/**
 *  自定义的保存游戏类
 */
UCLASS()
class MAGE_API UMageSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// 存档插槽名 
	UPROPERTY()
	FString LoadSlotName = FString(); 

	// 存档插槽索引
	UPROPERTY()
	int32 SlotIndex = 0;
	
	// 当前插槽的状态
	UPROPERTY()
	TEnumAsByte<ELoadSlotStatus> LoadSlotStatus = ELoadSlotStatus::Vacant;

	// 角色名称
	UPROPERTY()
	FString CharacterName = FString();

	// 起始点标签
	UPROPERTY()
	FName PlayerStartTag = FName();

	// 地图名称(UI显示)
	UPROPERTY()
	FString MapName = FString();

	// 地图资产名称
	UPROPERTY()
	FString MapAssetName = FString();

	// 判断是否初始化属性和赋予技能
	UPROPERTY()
	bool bFirstSaveGame = true;

	/** Player */
	UPROPERTY()
	int32 Level = 1;

	UPROPERTY()
	int32 XP = 0;

	UPROPERTY()
	int32 AttributePoints = 0;

	UPROPERTY()
	int32 SkillPoints = 0;

	/** Player Attributes */
	UPROPERTY()
	float Strength = 0.f;

	UPROPERTY()
	float Intelligence = 0.f;
	
	UPROPERTY()
	float Resilience = 0.f;

	UPROPERTY()
	float Vigor = 0.f;

	/** Player Abilities */
	UPROPERTY()
	TArray<FSavedAbilityData> SavedAbilitiesData;

	/** World State */
	UPROPERTY()
	TArray<FSavedWorld> SavedWorlds;

	FSavedWorld GetSavedWorldByWorldName(const FString& InWorldName);
	
	bool HasMap(const FString& InWorldName);
};
