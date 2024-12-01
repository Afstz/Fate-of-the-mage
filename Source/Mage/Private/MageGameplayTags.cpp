// Copyright AFstz.


#include "MageGameplayTags.h"
#include "GameplayTagsManager.h"

FMageGameplayTags FMageGameplayTags::MageGameplayTags;

void FMageGameplayTags::InitNativeGameplayTags()
{
	/** Primary Attribute*/
	MageGameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"), FString("Increase physical damage"));
	
	MageGameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"), FString("Increase magical damage and maximum of max mana"));
	
	MageGameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"), FString("Increase armor and armor penetration"));
	
	MageGameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"), FString("Increase maximum of max health"));
	
	/** Secondary Attribute*/
	MageGameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"), FString("Improve block chance, reduces damage taken"));
	
	MageGameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"), FString("Improve critical hit chance, ignore enemy armor"));
	
	MageGameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"), FString("Reduces half damage taken"));
	
	MageGameplayTags.Attributes_Secondary_CriticialHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticialHitChance"), FString("Impress critical hit chance"));
	
	MageGameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"), FString("Double damage"));
	
	MageGameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"), FString("Reduces enemy critical hit chance"));
	
	MageGameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"), FString("Life recovery per second"));
	
	MageGameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"), FString("Maximum of max health"));
	
	MageGameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"), FString("Mana recovery per second"));
	
	MageGameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"), FString("Maximum of max mana"));

	MageGameplayTags.Attributes_Secondary_PhysicalResistence = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.PhysicalResistence"), FString("Reduces physical attack damage"));
	MageGameplayTags.Attributes_Secondary_MagicalResistence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MagicalResistence"), FString("Magical Damage Resistence"));
	MageGameplayTags.Attributes_Secondary_FireResistence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.FireResistence"), FString("Fire Damage Resistence"));
	MageGameplayTags.Attributes_Secondary_LightningResistence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.LightningResistence"), FString("Lightning Damage Resistence"));
	

	/** Input Tags*/
	MageGameplayTags.Input_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.LMB"), FString("Left Mouse Button"));
	MageGameplayTags.Input_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.RMB"), FString("Right Mouse Button"));
	MageGameplayTags.Input_Q = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Q"), FString("Q key"));
	MageGameplayTags.Input_E = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.E"), FString("E key"));
	MageGameplayTags.Input_R = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.R"), FString("R key"));
	MageGameplayTags.Input_T = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.T"), FString("T key"));

	/** GameplayEffect Tags */
	MageGameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"), FString("Hit React"));
	
	/** Damage Type Tags */
	MageGameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"), FString("Damage Tag"));
	MageGameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Damage.Physical"), FString("Physical Damage Type"));
	MageGameplayTags.Damage_Magical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Magical"), FString("Magical Damage Type"));
	MageGameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Fire"), FString("Fire Damage Type"));
	MageGameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Lightning"), FString("Lightning Damage Type"));

	// 建立属性映射关系，便于计算
	MageGameplayTags.DamageTypesToResistence.Add(MageGameplayTags.Damage_Physical, MageGameplayTags.Attributes_Secondary_PhysicalResistence);
	MageGameplayTags.DamageTypesToResistence.Add(MageGameplayTags.Damage_Magical, MageGameplayTags.Attributes_Secondary_MagicalResistence);
	MageGameplayTags.DamageTypesToResistence.Add(MageGameplayTags.Damage_Fire, MageGameplayTags.Attributes_Secondary_FireResistence);
	MageGameplayTags.DamageTypesToResistence.Add(MageGameplayTags.Damage_Lightning, MageGameplayTags.Attributes_Secondary_LightningResistence);

	/** Abilites */
	MageGameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"), FString("Attack Ability Gameplay Tag"));
	MageGameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Summon"), FString("Summon Ability Gameplay Tag"));
	MageGameplayTags.Abilities_Fire_FireBall = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.FireBall"), FString("FireBall Ability Gameplay Tag"));

	/** Montage */
	MageGameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.1"), FString("Attack Montage Tag 1"));
	MageGameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.2"), FString("Attack Montage Tag 2"));
	MageGameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.3"), FString("Attack Montage Tag 3"));
	MageGameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.4"), FString("Attack Montage Tag 4"));
	/** Combat Tag */
	MageGameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Weapon"), FString("Weapon Socket"));
	MageGameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.LeftHand"), FString("LeftHand Socket"));
	MageGameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.RightHand"), FString("RightHand Socket"));
	MageGameplayTags.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("CombatSocket.Tail"), FString("Tail Socket"));
}