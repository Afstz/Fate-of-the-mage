// Copyright AFstz.


#include "AbilitySystem/GameplayAbility/DamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	const FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);

	for (TTuple<FGameplayTag, FScalableFloat>& Pair : DamageTypes) // 遍历所有设置的伤害类型
	{
		const float DamageMagnitude = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, DamageMagnitude);
	}
	// 对敌方造成伤害
	UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor)->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data.Get());
}