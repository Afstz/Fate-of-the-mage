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
	// 初始化本地自定义的游戏标签
    static void InitNativeGameplayTags(); 
	
	/* Primary Attribute Gameplay Tags. */
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
	FGameplayTag Input;
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
	FGameplayTag Effects_Infinite;

	/* Damage Type Tags */
	
	// 伤害类型对应伤害抗性
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistence; 
	FGameplayTag Damage;
	FGameplayTag Damage_Physical;
	FGameplayTag Damage_Magical;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;

	/* Debuff Tags */
	
	// 伤害类型对应Debuff
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs; 
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
	FGameplayTag Abilities_Fire_FireBlast;
	FGameplayTag Abilities_Lightning_LightningChain;
	FGameplayTag Abilities_Magical_ArcaneShards;
	FGameplayTag Abilities_HitReact;
	
	/* Passive Ability */
	FGameplayTag Abilities_Passive_HaloOfProtection;
	FGameplayTag Abilities_Passive_Unupgradable_LifeRegenerate;
	FGameplayTag Abilities_Passive_Unupgradable_ManaRegenerate;
	
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
	FGameplayTag Block_Player_AutoRun;

	/* Gameplay Cue Tag */
	FGameplayTag GameplayCue_FireBlast;

protected:
	
private:
    static FMageGameplayTags MageGameplayTags; // 全局单例
};
