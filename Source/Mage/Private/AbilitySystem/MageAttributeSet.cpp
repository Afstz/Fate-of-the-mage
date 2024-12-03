// Copyright AFstz.


#include "AbilitySystem/MageAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "MageGameplayTags.h"
#include "AbilitySystem/MageAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interface/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "..\..\LogMageChannels.h"
#include "Net/UnrealNetwork.h"
#include "Player/MagePlayerController.h"

UMageAttributeSet::UMageAttributeSet()
{
	FMageGameplayTags MageGameplayTags = FMageGameplayTags::Get();

	// Primary Attributes
	TagsToAttributes.Add(MageGameplayTags.Attributes_Primary_Strength, GetStrengthAttribute());
	TagsToAttributes.Add(MageGameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute());
	TagsToAttributes.Add(MageGameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute());
	TagsToAttributes.Add(MageGameplayTags.Attributes_Primary_Vigor, GetVigorAttribute());
 
	// Secondary Attributes
	TagsToAttributes.Add(MageGameplayTags.Attributes_Secondary_Armor, GetArmorAttribute());
	TagsToAttributes.Add(MageGameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute());
	TagsToAttributes.Add(MageGameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute());
	TagsToAttributes.Add(MageGameplayTags.Attributes_Secondary_CriticialHitChance, GetCriticalHitChanceAttribute());
	TagsToAttributes.Add(MageGameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute());
	TagsToAttributes.Add(MageGameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute());
	TagsToAttributes.Add(MageGameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute());
	TagsToAttributes.Add(MageGameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute());
	TagsToAttributes.Add(MageGameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute());
	TagsToAttributes.Add(MageGameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute());
	TagsToAttributes.Add(MageGameplayTags.Attributes_Secondary_PhysicalResistence, GetPhysicalResistenceAttribute());
	TagsToAttributes.Add(MageGameplayTags.Attributes_Secondary_MagicalResistence, GetMagicalResistenceAttribute());
	TagsToAttributes.Add(MageGameplayTags.Attributes_Secondary_FireResistence, GetFireResistenceAttribute());
	TagsToAttributes.Add(MageGameplayTags.Attributes_Secondary_LightningResistence, GetLightningResistenceAttribute());
}

void UMageAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 为了配合预测系统，让客户端的OnRep在属性相同的情况下(因为有预测)依旧回调
	DOREPLIFETIME_CONDITION_NOTIFY(UMageAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMageAttributeSet, Mana, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UMageAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMageAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMageAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMageAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UMageAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMageAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMageAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMageAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMageAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMageAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMageAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMageAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMageAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMageAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMageAttributeSet, PhysicalResistence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMageAttributeSet, MagicalResistence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMageAttributeSet, FireResistence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMageAttributeSet, LightningResistence, COND_None, REPNOTIFY_Always);
}

void UMageAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UMageAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	FEffectProperties EffectProps;
	SetEffectProperties(Data, EffectProps);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetReceivedDamageAttribute())
	{
		const float LocalDamage = GetReceivedDamage();
		SetReceivedDamage(0.f);
		if (LocalDamage >= 0.f)
		{
			float NewHealth = GetHealth() - LocalDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
			UE_LOG(LogMage, Warning, TEXT("Applied On: [%s],Health: [%f]"),*EffectProps.TargetCharacter->GetName(), GetHealth());
			bool bIsDie = GetHealth() <= 0.f;

			if (bIsDie)
			{
				if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProps.TargetAvatarActor))
				{
					CombatInterface->Die();
				}
			}
			else
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FMageGameplayTags::Get().Effects_HitReact);
				EffectProps.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}

			const bool bIsCriticalHit = UMageAbilitySystemLibrary::GetCriticalHit(EffectProps.SourceEffectContextHandle);
			const bool bIsBlockHit = UMageAbilitySystemLibrary::GetBlockHit(EffectProps.SourceEffectContextHandle);
			ShowCharacterDamageText(EffectProps, LocalDamage, bIsCriticalHit, bIsBlockHit);
		}
	}
}
void UMageAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProps)
{
	// Source 是效果的来源 Target 是效果的目标
	
	EffectProps.SourceEffectContextHandle = Data.EffectSpec.GetContext();
	EffectProps.SourceASC = EffectProps.SourceEffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if (IsValid(EffectProps.SourceASC) && EffectProps.SourceASC->AbilityActorInfo.IsValid() && EffectProps.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProps.SourceAvatarActor = EffectProps.SourceASC->AbilityActorInfo->AvatarActor.Get();
		EffectProps.SourceCharacter = Cast<ACharacter>(EffectProps.SourceAvatarActor);
		EffectProps.SourceController = EffectProps.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (EffectProps.SourceController == nullptr && EffectProps.SourceAvatarActor != nullptr)
		{
			EffectProps.SourceController = EffectProps.SourceCharacter->GetController();
		}
	}
	
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProps.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		EffectProps.TargetCharacter = Cast<ACharacter>(EffectProps.TargetAvatarActor);
		EffectProps.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		EffectProps.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EffectProps.TargetAvatarActor);
	}
}

void UMageAttributeSet::ShowCharacterDamageText(const FEffectProperties& EffectProperties, const float DamageValue, const bool bIsCriticalHit, const bool bIsBlockHit) const 
{
	if (EffectProperties.SourceCharacter != EffectProperties.TargetCharacter) // 对别人造成伤害才显示浮动伤害文本
	{
		if (AMagePlayerController* MagePC = Cast<AMagePlayerController>(EffectProperties.SourceController))
		{
			// 自己对造成敌人伤害
			MagePC->ShowDamageText(EffectProperties.TargetCharacter, DamageValue, bIsCriticalHit, bIsBlockHit);
			return;
		}
		if (AMagePlayerController* MagePC = Cast<AMagePlayerController>(EffectProperties.TargetController))
		{
			// 敌人对自己造成伤害
			MagePC->ShowDamageText(EffectProperties.TargetCharacter, DamageValue, bIsCriticalHit, bIsBlockHit);
		}
	}
}

void UMageAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMageAttributeSet, Health, OldHealth); // 为了预测给ASC提供数据
}

void UMageAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMageAttributeSet, Mana, OldMana);
}

void UMageAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMageAttributeSet, Strength, OldStrength);
}

void UMageAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMageAttributeSet, Intelligence, OldIntelligence);
}

void UMageAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMageAttributeSet, Resilience, OldResilience);
}

void UMageAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMageAttributeSet, Vigor, OldVigor);
}

void UMageAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMageAttributeSet, Armor, OldArmor);
}

void UMageAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMageAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UMageAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMageAttributeSet, BlockChance, OldBlockChance);
}

void UMageAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMageAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UMageAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMageAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UMageAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMageAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UMageAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMageAttributeSet, HealthRegeneration, OldHealthRegeneration);
}
void UMageAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMageAttributeSet, MaxHealth, OldMaxHealth);
}

void UMageAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMageAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UMageAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMageAttributeSet, MaxMana, OldMaxMana);
}

void UMageAttributeSet::OnRep_PhysicalResistence(const FGameplayAttributeData& OldPhysicalResistence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMageAttributeSet, PhysicalResistence, OldPhysicalResistence);
}

void UMageAttributeSet::OnRep_MagicalResistence(const FGameplayAttributeData& OldMagicalResistence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMageAttributeSet, MagicalResistence, OldMagicalResistence);
}

void UMageAttributeSet::OnRep_FireResistence(const FGameplayAttributeData& OldFireResistence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMageAttributeSet, FireResistence, OldFireResistence);
}

void UMageAttributeSet::OnRep_LightningResistence(const FGameplayAttributeData& OldLightningResistence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMageAttributeSet, LightningResistence, OldLightningResistence);
}