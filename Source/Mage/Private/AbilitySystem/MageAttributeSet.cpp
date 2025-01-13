// Copyright AFstz.


#include "AbilitySystem/MageAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "MageAbilityTypes.h"
#include "MageGameplayTags.h"
#include "AbilitySystem/MageAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Interface/CombatInterface.h"
#include "Interface/PlayerInterface.h"
#include "Mage/LogMageChannels.h"
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
	SetEffectProperties(Data, EffectProps);;

	// 死亡则不进行计算处理
	if (EffectProps.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(EffectProps.TargetCharacter)) return; 

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
		HandleReceivedDamage(EffectProps);
	}
	if (Data.EvaluatedData.Attribute == GetReceivedXPAttribute())
	{
		HandleReceivedXP(EffectProps);
	}
}

void UMageAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProps)
{
	// Source 是效果的来源 Target 是效果的目标
	
	EffectProps.SourceEffectContextHandle = Data.EffectSpec.GetContext();
	EffectProps.SourceEffectSpec = Data.EffectSpec;
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

void UMageAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() && bFillInHealth)
	{
		SetHealth(GetMaxHealth());
		bFillInHealth = false;
	}
	if (Attribute == GetMaxManaAttribute() && bFillInMana)
	{
		SetMana(GetMaxMana());
		bFillInMana = false;
	}
}

void UMageAttributeSet::HandleReceivedDamage(const FEffectProperties& EffectProps)
{
	const float LocalDamage = GetReceivedDamage();
	SetReceivedDamage(0.f);
	if (LocalDamage >= 0.f)
	{
		float NewHealth = GetHealth() - LocalDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
		bool bDie = GetHealth() <= 0.f;

		if (bDie)
		{
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProps.TargetAvatarActor))
			{
				const FVector& DeathImpulse = UMageAbilitySystemLibrary::GetDeathImpulse(EffectProps.SourceEffectContextHandle); // 死亡冲击力
				CombatInterface->Die(DeathImpulse);
			}
			SendXPRewardEvent(EffectProps);
		}
		else
		{
			FGameplayTagContainer Tags;
			EffectProps.SourceEffectSpec.GetAllGrantedTags(Tags);
			if (!Tags.HasTag(FGameplayTag::RequestGameplayTag(FName("Debuff")))) // 没有Debuff才播放受击动画
			{
				// HitReact Ability
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FMageGameplayTags::Get().Effects_HitReact);
				EffectProps.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}
			// 击退
			const FVector& KnockbackForce = UMageAbilitySystemLibrary::GetKnockbackForce(EffectProps.SourceEffectContextHandle);
			if (!KnockbackForce.IsNearlyZero(0.5f))
			{
				EffectProps.TargetCharacter->LaunchCharacter(KnockbackForce, true, true);
			}
		}

		const bool bIsCriticalHit = UMageAbilitySystemLibrary::GetCriticalHit(EffectProps.SourceEffectContextHandle);
		const bool bIsBlockHit = UMageAbilitySystemLibrary::GetBlockHit(EffectProps.SourceEffectContextHandle);
		ShowCharacterDamageText(EffectProps, LocalDamage, bIsCriticalHit, bIsBlockHit);
		
		if (UMageAbilitySystemLibrary::GetSuccessfulDebuff(EffectProps.SourceEffectContextHandle))
		{
			HandleDebuff(EffectProps);
		}
	}
}

void UMageAttributeSet::HandleDebuff(const FEffectProperties& EffectProps)
{
	// 创建实际施加的Debuff Effect Class
	const FMageGameplayTags& MageGameplayTags = FMageGameplayTags::Get();
	FGameplayEffectContextHandle DebuffContextHandle = EffectProps.SourceASC->MakeEffectContext();
	DebuffContextHandle.AddSourceObject(EffectProps.SourceASC->GetAvatarActor());
	
	const FGameplayTag& DamageType = UMageAbilitySystemLibrary::GetDamageType(EffectProps.SourceEffectContextHandle);
	const float DebuffDamage = UMageAbilitySystemLibrary::GetDebuffDamage(EffectProps.SourceEffectContextHandle);
	const float DebuffDuration = UMageAbilitySystemLibrary::GetDebuffDuration(EffectProps.SourceEffectContextHandle);
	const float DebuffFrequence = UMageAbilitySystemLibrary::GetDebuffFrequence(EffectProps.SourceEffectContextHandle);
	
	FString EffectName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageType.ToString());
	UGameplayEffect* GameplayEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(EffectName));

	// Effect属性设置
	GameplayEffect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	GameplayEffect->DurationMagnitude = FScalableFloat(DebuffDuration);
	GameplayEffect->Period = DebuffFrequence;
	GameplayEffect->bExecutePeriodicEffectOnApplication = false;
	GameplayEffect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	GameplayEffect->StackLimitCount = 1;
	// 向Effect添加赋予标签
	UTargetTagsGameplayEffectComponent& TargetTagsComp = GameplayEffect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritedTagContainer;
	InheritedTagContainer.AddTag(MageGameplayTags.DamageTypesToDebuffs[DamageType]);
	TargetTagsComp.SetAndApplyTargetTagChanges(InheritedTagContainer);

	// 添加要修改的属性
	int Index = GameplayEffect->Modifiers.Num();
	GameplayEffect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = GameplayEffect->Modifiers[Index];
	ModifierInfo.Attribute = GetReceivedDamageAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);

	// 创建对应的Spec
	FGameplayEffectSpec* DebuffEffectSpec = new FGameplayEffectSpec(GameplayEffect, DebuffContextHandle, 1.f);
	if (DebuffEffectSpec)
	{
		// 新创建的Effect要重新添加一次标签
		FMageGameplayEffectContext* MageDebuffContextHandle = static_cast<FMageGameplayEffectContext*>(DebuffContextHandle.Get());
		TSharedPtr<FGameplayTag> InDamageType = MakeShareable(new FGameplayTag(DamageType));
		MageDebuffContextHandle->SetDamageType(InDamageType);
		EffectProps.TargetASC->ApplyGameplayEffectSpecToSelf(*DebuffEffectSpec);
	}
}

void UMageAttributeSet::HandleReceivedXP(const FEffectProperties& EffectProps)
{
	float LocalReceivedXP = GetReceivedXP();
	SetReceivedXP(0.f);
	if (EffectProps.SourceCharacter->Implements<UPlayerInterface>() && EffectProps.SourceCharacter->Implements<UCombatInterface>())
	{
		const int32 CurrentLevel = ICombatInterface::Execute_GetCharacterLevel(EffectProps.SourceCharacter);
		const int32 CurrentXP = IPlayerInterface::Execute_GetXP(EffectProps.SourceCharacter);
		// CurrentXP + LocalReceivedXP 是在原有的总经验值基础上计算等级
		int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(EffectProps.SourceCharacter, CurrentXP + LocalReceivedXP);
		int32 LevelUpCount = NewLevel - CurrentLevel; // 总升级数

		if (LevelUpCount > 0) // 有升级就执行升级的逻辑
		{
			int32 AttributePoint = IPlayerInterface::Execute_GetAttributePointReward(
				EffectProps.SourceCharacter, NewLevel);
			int32 SkillPoint = IPlayerInterface::Execute_GetSkillPointReward(EffectProps.SourceCharacter, NewLevel);

			IPlayerInterface::Execute_AddToAttributePoint(EffectProps.SourceCharacter, AttributePoint * LevelUpCount);
			IPlayerInterface::Execute_AddToSkillPoint(EffectProps.SourceCharacter, SkillPoint * LevelUpCount);
			IPlayerInterface::Execute_AddToLevel(EffectProps.SourceCharacter, LevelUpCount);

			bFillInHealth = true; // 延迟填充，因为等级升级后最大值会重新进行MMC计算
			bFillInMana = true;

			IPlayerInterface::Execute_LevelUp(EffectProps.SourceCharacter);
		}
		IPlayerInterface::Execute_AddToXP(EffectProps.SourceCharacter, LocalReceivedXP); // 向玩家添加经验
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

void UMageAttributeSet::SendXPRewardEvent(const FEffectProperties& EffectProperties)
{
	if (EffectProperties.TargetCharacter->Implements<UCombatInterface>())
	{
		// 根据敌人等级和类型获取经验
		const int32 TargetCharacterLevel = ICombatInterface::Execute_GetCharacterLevel(EffectProperties.TargetCharacter);
		const ECharacterClass CharacterClass = ICombatInterface::Execute_GetCharacterClass(EffectProperties.TargetCharacter);
		int32 XPReward = UMageAbilitySystemLibrary::GetXPRewardForClassAndLevel(EffectProperties.TargetCharacter, CharacterClass, TargetCharacterLevel);

		const FMageGameplayTags& MageGameplayTags = FMageGameplayTags::Get();
		FGameplayTag EventTag = MageGameplayTags.Attributes_Meta_ReceivedXP; // 要发送的事件Tag
	
		FGameplayEventData Payload;
		Payload.EventTag = EventTag;
		Payload.EventMagnitude = XPReward;
		// 向玩家发送事件
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(EffectProperties.SourceCharacter, EventTag, Payload);
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