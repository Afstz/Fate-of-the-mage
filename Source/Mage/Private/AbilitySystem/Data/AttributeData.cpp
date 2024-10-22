// Copyright AFstz.


#include "AbilitySystem/Data/AttributeData.h"

FMageAttributeData UAttributeData::FindAttributeDataForTag(const FGameplayTag& AttributeTag) const
{
	for (const FMageAttributeData& Data : AttributesData)
	{
		if (Data.AttributeTag.IsValid() && Data.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Data;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Not Find [%s] from [%s]"), *AttributeTag.ToString(), *GetNameSafe(this));
	return FMageAttributeData();
}