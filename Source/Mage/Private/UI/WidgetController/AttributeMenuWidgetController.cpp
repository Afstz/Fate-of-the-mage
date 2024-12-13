// Copyright AFstz.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/MageAttributeSet.h"
#include "AbilitySystem/Data/AttributeData.h"

void UAttributeMenuWidgetController::BroadcastInitialValue()
{
	UMageAttributeSet* AS = Cast<UMageAttributeSet>(AttributeSet);

	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeData(Pair.Key, Pair.Value);
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UMageAttributeSet* AS = Cast<UMageAttributeSet>(AttributeSet);
	
	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeData(Pair.Key, Pair.Value);
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeData(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	check(AttributeData);
	FMageAttributeData FoundAttributeData = AttributeData->FindAttributeDataForTag(AttributeTag);
	FoundAttributeData.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeMenuDataDelegate.Broadcast(FoundAttributeData);
}