// Copyright AFstz.


#include "Input/MageInputData.h"

const UInputAction* UMageInputData::FindInputActionForTag(const FGameplayTag& FindInputTag)
{
	for (const FMageInputActionData& InputData : AbilityInputActions)
	{
		if (InputData.InputGameplayTag.IsValid() && InputData.InputGameplayTag.MatchesTagExact(FindInputTag))
		{
			return  InputData.InputAction;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Not Find Tag:[%s] from [%s]"), *FindInputTag.ToString(), *GetNameSafe(this));
	return  nullptr;
}