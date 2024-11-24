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
    static void InitNativeGameplayTags(); // 初始化本地自定义的游戏标签
	
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

	FGameplayTag Attributes_Secondary_PhysicalResistence;
	FGameplayTag Attributes_Secondary_MagicalResistence;
	FGameplayTag Attributes_Secondary_FireResistence;
	FGameplayTag Attributes_Secondary_LightningResistence;
	
	/** Input Tags */
	FGameplayTag Input_LMB;
	FGameplayTag Input_RMB;
	FGameplayTag Input_1;
	FGameplayTag Input_2;
	FGameplayTag Input_3;
	FGameplayTag Input_4;

	/** GameplayEffect Tags */
	FGameplayTag Effects_HitReact;

	/** Damage Type Tags */
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistence; // 伤害属性对应伤害抗性
	FGameplayTag Damage;
	FGameplayTag Damage_Physical;
	FGameplayTag Damage_Magical;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;

	/** Abilites */
	FGameplayTag Attack;

	/** Montage */
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;
	/** CombatSocket Tag */
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_Tail;
protected:
	
private:
    static FMageGameplayTags MageGameplayTags; // 全局单例
};
