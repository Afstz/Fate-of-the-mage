// Copyright AFstz.


#include "AbilitySystem/Data/AttributeData.h"
#include "Mage/LogMageChannels.h"

FMageAttributeData UAttributeData::FindAttributeDataForTag(const FGameplayTag& AttributeTag, bool bWarnIfNotFound) const
{
	for (const FMageAttributeData& Data : AttributesData)
	{
		if (Data.AttributeTag.IsValid() && Data.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Data;
		}
	}

	if (bWarnIfNotFound)
	{
		UE_LOG(LogMage, Error, TEXT("Not Find [%s] from [%s]"), *AttributeTag.ToString(), *GetNameSafe(this));
	}
	return FMageAttributeData();
}