// Copyright AFstz.


#include "AbilitySystem/MageAbilitySystemComponent.h"

#include "MageGameplayTags.h"
#include "AbilitySystem/GameplayAbility/MageGameplayAbility.h"

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
	for (TSubclassOf<UGameplayAbility>& AbilityClass : AbilityClasses)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (UMageGameplayAbility* MageGameplayAbility = Cast<UMageGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(MageGameplayAbility->StartupTag); // 把技能标签添加到动态标签容器中
			GiveAbility(AbilitySpec); // 赋予技能但不执行
		}
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