// Copyright AFstz.


#include "Input/MageInputData.h"
#include "Mage/LogMageChannels.h"

const UInputAction* UMageInputData::FindInputActionForTag(const FGameplayTag& FindInputTag, bool bWarnIfNotFound)
{
	for (const FMageInputActionData& InputData : AbilityInputActions)
	{
		if (InputData.InputGameplayTag.IsValid() && InputData.InputGameplayTag.MatchesTagExact(FindInputTag))
		{
			return InputData.InputAction;
		}
	}

	if (bWarnIfNotFound)
	{
		UE_LOG(LogMage, Error, TEXT("Not Find Tag:[%s] from [%s]"), *FindInputTag.ToString(), *GetNameSafe(this));
	}
	return  nullptr;
}