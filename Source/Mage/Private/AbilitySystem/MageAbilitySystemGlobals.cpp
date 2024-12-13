// Copyright AFstz.


#include "AbilitySystem/MageAbilitySystemGlobals.h"

#include "MageAbilityTypes.h"

FGameplayEffectContext* UMageAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FMageGameplayEffectContext();
}