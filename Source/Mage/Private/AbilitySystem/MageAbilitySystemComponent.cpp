// Copyright AFstz.


#include "AbilitySystem/MageAbilitySystemComponent.h"

void UMageAbilitySystemComponent::AbilityActorInfoIsSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::OnEffectApplied);
}

void UMageAbilitySystemComponent::OnEffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                  const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer GameplayTagContainer;
	EffectSpec.GetAllAssetTags(GameplayTagContainer);
	EffectAssetTagsDelegate.Broadcast(GameplayTagContainer);
}