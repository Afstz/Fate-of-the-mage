// Copyright AFstz.

#include "AbilitySystem/MageAbilitySystemComponent.h"
#include "MageGameplayTags.h"
#include "AbilitySystem/GameplayAbility/MageGameplayAbility.h"
#include "Interface/CombatInterface.h"
#include "Mage/LogMageChannels.h"

void UMageAbilitySystemComponent::AbilityActorInfoIsSet()
{
	// 绑定的回调函数只在服务器执行
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::ClientOnEffectApplied);
}

void UMageAbilitySystemComponent::ClientOnEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                  const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer GameplayTagContainer;
	EffectSpec.GetAllAssetTags(GameplayTagContainer);
	EffectAssetTagsDelegate.Broadcast(GameplayTagContainer);
}

void UMageAbilitySystemComponent::AddCharacterAbilites(TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses)
{
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : AbilityClasses)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 10);
		if (UMageGameplayAbility* MageGameplayAbility = Cast<UMageGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(MageGameplayAbility->StartupInputTag); // 把技能标签添加到动态标签容器中
			GiveAbility(AbilitySpec); // 赋予技能但不执行
		}
	}
	// 初始化技能完毕，给控制层发数据
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast(this);
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
	for (const FGameplayTag& Tag : AbilitySpec.DynamicAbilityTags) // DynamicAbilityTags存储着StartupInputTag
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Input"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

void UMageAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	// 技能在服务器上赋予所以得手动同步一下
	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast(this);
	}
}