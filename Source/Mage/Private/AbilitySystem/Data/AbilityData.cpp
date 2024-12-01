// Copyright AFstz.


#include "AbilitySystem/Data/AbilityData.h"

#include "Mage/MageLogChannels.h"

FMageAbilityData UAbilityData::FindAbilityDataForTag(const FGameplayTag& AbilityTag, bool bWarnIfNotFound)
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
		UE_LOG(MageLog, Error, TEXT("Not find [%s] from [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}
	return FMageAbilityData();
}