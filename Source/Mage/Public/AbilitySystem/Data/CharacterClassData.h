// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "CharacterClassData.generated.h"


class UGameplayAbility;
class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Warrior, // 战士
	Ranger, // 游侠
	Mage // 法师
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DefaultPrimaryEffects; // 自定义的初始属性
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> CharacterDefaultAbilites; // 角色默认技能
	UPROPERTY(EditDefaultsOnly)
	FScalableFloat XPRewardTable = FScalableFloat();
};

/**
 *  存储默认角色类型的相关信息
 */
UCLASS()
class MAGE_API UCharacterClassData : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> DefaultCharacterInfo; // 与相关角色类型相匹配的初始属性

	// 共享的数据
	UPROPERTY(EditDefaultsOnly, Category = "Shared Class Defaults")
	TSubclassOf<UGameplayEffect> DefaultSecondEffects;
	UPROPERTY(EditDefaultsOnly, Category = "Shared Class Defaults")
	TSubclassOf<UGameplayEffect> DefaultBaseEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;
	
	FCharacterClassDefaultInfo FindCharacterClassInfo(const ECharacterClass CharacterClass);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TObjectPtr<UCurveTable> DamageCoefficientTables;
};
