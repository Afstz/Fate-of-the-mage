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

	/** Meta Attribute */
	MageGameplayTags.Attributes_Meta_ReceivedXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.ReceivedXP"), FString("ReceivedXP Meta Attribute Tag"));

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
	MageGameplayTags.Input_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Passive.1"), FString("Passive Skill 1"));
	MageGameplayTags.Input_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Passive.2"), FString("Passive Skill 2"));

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
	
	/** Debuff Tags */
	MageGameplayTags.Debuff_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Debuff.Physical"), FString("Physical Debuff"));
	MageGameplayTags.Debuff_Magical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Magical"), FString("Magical Debuff"));
	MageGameplayTags.Debuff_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Burn"), FString("Burn Debuff"));
	MageGameplayTags.Debuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Stun"), FString("Stun Debuff"));
	MageGameplayTags.Debuff_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Chance"), FString("Debuff Chance"));
	MageGameplayTags.Debuff_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Damage"), FString("Debuff Damage"));
	MageGameplayTags.Debuff_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Duration"), FString("Debuff Duration"));
	MageGameplayTags.Debuff_Frequence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Frequence"), FString("Debuff Frequence"));
	// 建立属性映射关系，便于计算
	MageGameplayTags.DamageTypesToDebuffs.Add(MageGameplayTags.Damage_Physical, MageGameplayTags.Debuff_Physical);
	MageGameplayTags.DamageTypesToDebuffs.Add(MageGameplayTags.Damage_Magical, MageGameplayTags.Debuff_Magical);
	MageGameplayTags.DamageTypesToDebuffs.Add(MageGameplayTags.Damage_Fire, MageGameplayTags.Debuff_Burn);
	MageGameplayTags.DamageTypesToDebuffs.Add(MageGameplayTags.Damage_Lightning, MageGameplayTags.Debuff_Stun);
	
	/** Abilites */
	MageGameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.None"), FString("None Tag. like the nullptr"));
	MageGameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"), FString("Attack Ability Gameplay Tag"));
	MageGameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Summon"), FString("Summon Ability Gameplay Tag"));
	MageGameplayTags.Abilities_Fire_FireBall = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.FireBall"), FString("FireBall Ability Gameplay Tag"));
	MageGameplayTags.Abilities_Fire_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.FireBlast"), FString("FireBlast Ability Gameplay Tag"));
	MageGameplayTags.Abilities_Lightning_LightningChain = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Lightning.LightningChain"), FString("LightningChain Ability Gameplay Tag"));
	MageGameplayTags.Abilities_Magical_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Magical.ArcaneShards"), FString("ArcaneShards Ability Gameplay Tag"));
	MageGameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.HitReact"), FString("Hit React Ability Tag"));
	/** Passive Abilities */
	MageGameplayTags.Abilities_Passive_HaloOfProtection = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.HaloOfProtection"), FString("Halo Of Protection"));
	MageGameplayTags.Abilities_Passive_Unupgradable_LifeRegenerate = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.Unupgradable.LifeRegenerate"), FString("Unupgradable Life Regenerate"));
	MageGameplayTags.Abilities_Passive_Unupgradable_ManaRegenerate = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.Unupgradable.ManaRegenerate"), FString("Unupgradable Mana Regenerate"));
	/** Abilities Status */
	MageGameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Status.Unlocked"), FString("Unlocked Status Ability Tag"));
	MageGameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Status.Eligible"), FString("Eligible Status Ability Tag"));
	MageGameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Status.Locked"), FString("Locked Status Ability Tag"));
	MageGameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Status.Equipped"), FString("Equipped Status Ability Tag"));
	/** Abilities Type */
	MageGameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Type.None"), FString("None Type Ability Tag"));
	MageGameplayTags.Abilities_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Abilities.Type.Offensive"), FString("Offensive Type Ability Tag"));
	MageGameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Passive"), FString("Passive Type Ability Tag"));
	
	/** Cooldown */
	MageGameplayTags.Cooldown_Fire_FireBall = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Fire.FireBall"), FString("FireBall Cooldown Tag"));

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

	/** Block Tag */
	MageGameplayTags.Block_Player_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Block.Player.CursorTrace"), FString("Block Cursor Tracing"));
	MageGameplayTags.Block_Player_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Block.Player.InputPressed"), FString("Block Input Pressed callback"));
	MageGameplayTags.Block_Player_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Block.Player.InputHeld"), FString("Block Input InputHeld callback"));
	MageGameplayTags.Block_Player_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Block.Player.InputReleased"), FString("Block Input InputReleased callback"));
	MageGameplayTags.Block_Player_AutoRun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Block.Player.AutoRun"), FString("Block AutoRun Function"));

	/** Gameplay Cue Tag */
	MageGameplayTags.GameplayCue_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.FireBlast"), FString("Fire Blast Gameplay Cue Tag"));
}