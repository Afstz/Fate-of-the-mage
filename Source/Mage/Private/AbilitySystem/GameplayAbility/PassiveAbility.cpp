// Copyright AFstz.


#include "AbilitySystem/GameplayAbility/PassiveAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/MageAbilitySystemComponent.h"

FString UPassiveAbility::GetDescription(int32 AbilityLevel)
{
	return FString::Printf(TEXT(
		// Title
		"<Title>%s</>\n\n"
		"<Default>技能等级:</><Level>%d</>\n\n"
		// Description
		"<Small>%s</>")
		
		, *AbilityName, AbilityLevel, *AbilityDescription);
}

FString UPassiveAbility::GetNextLevelDescription(int32 AbilityLevel)
{
	return !bCanUpgrade ? FString() : FString::Printf(TEXT(
		// Title
		"<Title>%s</>\n\n"
		"<Default>技能等级:</><Level>%d</>\n\n"
		// Description
		"<Small>提升此被动技能的等级，增强技能效果。</>")
		
		, *AbilityName, AbilityLevel);
}

void UPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UMageAbilitySystemComponent* MageASC = Cast<UMageAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		MageASC->DeactivatePassiveDelegate.AddUObject(this, &ThisClass::OnAbilityDeactivate);
		ApplyPassiveGameplayEffect();
	}
}

void UPassiveAbility::OnAbilityDeactivate(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		RemovePassiveGameplayEffect();
		
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UPassiveAbility::ApplyPassiveGameplayEffect()
{
	check(PassiveEffectClass);
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(PassiveEffectClass, GetAbilityLevel(), ContextHandle);
	
	ActiveEffectHandle = GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void UPassiveAbility::RemovePassiveGameplayEffect()
{
	if (ActiveEffectHandle.IsValid()) // 移除InfiniteEffect
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(ActiveEffectHandle);
	}
}