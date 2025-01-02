// Copyright AFstz.


#include "AbilitySystem/Data/AbilityData.h"
#include "Mage/LogMageChannels.h"

FMageAbilityData UAbilityData::FindAbilityDataForTag(const FGameplayTag& AbilityTag, bool bWarnIfNotFound) const
{
	for (const FMageAbilityData& Data : AbilitiesData)
	{
		if (Data.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return Data;
		}
	}

	if (bWarnIfNotFound)
	{
		UE_LOG(LogMage, Error, TEXT("Not find [%s] from [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}
	return FMageAbilityData();
}