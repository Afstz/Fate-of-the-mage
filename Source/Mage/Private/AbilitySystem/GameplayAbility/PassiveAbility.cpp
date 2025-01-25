// Copyright AFstz.


#include "AbilitySystem/GameplayAbility/PassiveAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/MageAbilitySystemComponent.h"

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