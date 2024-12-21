// Copyright AFstz.

#include "AbilitySystem/MageAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "MageGameplayTags.h"
#include "AbilitySystem/MageAbilitySystemLibrary.h"
#include "AbilitySystem/Data/AbilityData.h"
#include "AbilitySystem/GameplayAbility/MageGameplayAbility.h"
#include "Interface/PlayerInterface.h"
#include "Mage/LogMageChannels.h"

void UMageAbilitySystemComponent::AbilityActorInfoIsSet()
{
	// 绑定的回调函数只在服务器执行
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::ClientOnEffectApplied);
}

void UMageAbilitySystemComponent::ClientOnEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer GameplayTagContainer;
	EffectSpec.GetAllAssetTags(GameplayTagContainer);
	EffectAssetTagsDelegate.Broadcast(GameplayTagContainer);
}

void UMageAbilitySystemComponent::AddCharacterAbilites(TArray<TSubclassOf<UGameplayAbility>>& AbilitiesClass)
{
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : AbilitiesClass)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 10);
		if (UMageGameplayAbility* MageGameplayAbility = Cast<UMageGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(MageGameplayAbility->StartupInputTag); // 把技能标签添加到动态标签容器中
			AbilitySpec.DynamicAbilityTags.AddTag(FMageGameplayTags::Get().Abilities_Status_Equipped);
			GiveAbility(AbilitySpec); // 赋予技能但不执行
		}
	}
	// 初始化技能完毕，给控制层发数据
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UMageAbilitySystemComponent::AddCharacterPassiveAbilites(TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilityClasses)
{
	for (const TSubclassOf<UGameplayAbility>& PassiveAbility : PassiveAbilityClasses)
	{
		FGameplayAbilitySpec PassiveAbilitySpec = FGameplayAbilitySpec(PassiveAbility);
		GiveAbilityAndActivateOnce(PassiveAbilitySpec);
	}
}

void UMageAbilitySystemComponent::AbilityInputHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) // 获取所以可激活的技能数组
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) // 从动态标签容器中找是否有匹配标签
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive()) // 如果没有激活技能就尝试激活
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UMageAbilitySystemComponent::AbilityInputReleased(const FGameplayTag& GameplayTag)
{
	if (!GameplayTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(GameplayTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UMageAbilitySystemComponent::ForeachAbilitiesExecute(const FForeachAbilitiesSignature& ForeachDelegate)
{
	// 用于在我们遍历能力时，阻止我们从能力系统组件中移除能力(加锁,保证线程安全)
	FScopedAbilityListLock ScopedAbilityListLock(*this);
	
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!ForeachDelegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogMage, Error, TEXT("Failed to execute on [%hs] function"), __FUNCTION__);
		}
	}
}

FGameplayTag UMageAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (const FGameplayTag& AbilityTag : AbilitySpec.Ability->AbilityTags)
		{
			if (AbilityTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return AbilityTag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UMageAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (const FGameplayTag& InputTag : AbilitySpec.DynamicAbilityTags) // DynamicAbilityTags存储着StartupInputTag
	{
		if (InputTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Input"))))
		{
			return InputTag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UMageAbilitySystemComponent::GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (const FGameplayTag& StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UMageAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ScopedAbilityListLock(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (const FGameplayTag& Tag : AbilitySpec.Ability->AbilityTags)
		{
			if (AbilityTag.MatchesTagExact(Tag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

void UMageAbilitySystemComponent::UpdateAbilityStatuses(const int32 InPlayerLevel)
{
	UAbilityData* AbilityData = UMageAbilitySystemLibrary::GetAbilityData(this);
	for (const FMageAbilityData& Data : AbilityData->AbilitiesData) // 寻找符合等级的技能
	{
		if (!Data.AbilityTag.IsValid()) continue;
		if (Data.AbilityLevelRequirement > InPlayerLevel) continue;
		if (GetSpecFromAbilityTag(Data.AbilityTag) == nullptr) // 没有赋予这个技能
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Data.AbilityClass, 1.f);
			AbilitySpec.DynamicAbilityTags.AddTag(FMageGameplayTags::Get().Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec); // 无需等待下次更新，直接复制到客户端
			ClientUpdateAbilityStatus(Data.AbilityTag, FMageGameplayTags::Get().Abilities_Status_Eligible);
		}
	}
}

void UMageAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
{
	AbilityStatusChanged.Broadcast(AbilityTag, StatusTag); // 同步技能状态
}

void UMageAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoint(GetAvatarActor()) > 0) // 防止发送垃圾数据
		{
			ServerUpgradeAttribute(AttributeTag); // 保证在服务器对属性进行操作
		}
	}
}

void UMageAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoint(GetAvatarActor()) > 0) // 服务器再次进行验证
		{
			FGameplayEventData Payload;
			Payload.EventTag = AttributeTag;
			Payload.EventMagnitude = 1.f;
	
			// 向自身发送事件，通过被动事件监听技能接收并给属性加点
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);
			
			IPlayerInterface::Execute_AddToAttributePoint(GetAvatarActor(), -1);
		}
	}
}

void UMageAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	// 技能在服务器上赋予所以得手动同步一下
	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}