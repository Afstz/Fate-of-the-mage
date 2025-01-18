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
	
	/* Primary Attribute Gameplay Tags*/
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;
	
	/* Secondary Attribute Gameplay Tags*/
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

	/* Meta Attribute */
	FGameplayTag Attributes_Meta_ReceivedXP;
	
	/* Input Tags */
	FGameplayTag Input_LMB;
	FGameplayTag Input_RMB;
	FGameplayTag Input_Q;
	FGameplayTag Input_E;
	FGameplayTag Input_R;
	FGameplayTag Input_T;
	FGameplayTag Input_Passive_1;
	FGameplayTag Input_Passive_2;

	/* GameplayEffect Tags */
	FGameplayTag Effects_HitReact;

	/* Damage Type Tags */
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistence; // 伤害属性对应伤害抗性
	FGameplayTag Damage;
	FGameplayTag Damage_Physical;
	FGameplayTag Damage_Magical;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;

	/* Debuff Tags */
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs; // 伤害属性对应Debuff
	FGameplayTag Debuff_Physical;
	FGameplayTag Debuff_Magical;
	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Stun;
	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Duration;
	FGameplayTag Debuff_Frequence;
	
	/* Abilites */
	FGameplayTag Abilities_None;
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	FGameplayTag Abilities_Fire_FireBall;
	FGameplayTag Abilities_Lightning_LightningChain;
	FGameplayTag Abilities_HitReact;
	/* Abilities Status */
	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;
	/* Abilities Type */
	FGameplayTag Abilities_Type_None;
	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;

	/* Cooldown */
	FGameplayTag Cooldown_Fire_FireBall;
	
	/* Montage */
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;
	
	/* CombatSocket Tag */
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_Tail;

	/* Block Tag */
	FGameplayTag Block_Player_CursorTrace;
	FGameplayTag Block_Player_InputPressed;
	FGameplayTag Block_Player_InputHeld;
	FGameplayTag Block_Player_InputReleased;
protected:
	
private:
    static FMageGameplayTags MageGameplayTags; // 全局单例
};
