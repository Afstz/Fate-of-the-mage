// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * MageGameplayTags 单例
 * 包含本地自定义的游戏标签
 */

struct FMageGameplayTags
{
public:
    static FMageGameplayTags& Get() { return MageGameplayTags;}
    static void InitNativeGameplayTags(); // 加载本地自定义的游戏标签
	
	/** Primary Attribute Gameplay Tags*/
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;
	
	/** Secondary Attribute Gameplay Tags*/
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticialHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxMana;
protected:
	
private:
    static FMageGameplayTags MageGameplayTags; // 全局单例
};
