// Copyright AFstz.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/MageAttributeSet.h"
#include "AbilitySystem/Data/AttributeData.h"

void UAttributeMenuWidgetController::BroadcastInitialValue()
{
	UMageAttributeSet* AS = Cast<UMageAttributeSet>(AttributeSet);
	check(AttributeDatas);

	for (auto& Pair : AS->TagsForAttributeMap)
	{
		BroadcastAttributeData(Pair.Key, Pair.Value);
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UMageAttributeSet* AS = Cast<UMageAttributeSet>(AttributeSet);
	check(AttributeDatas);

	for (auto& Pair : AS->TagsForAttributeMap)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeData(Pair.Key, Pair.Value);
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeData(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	FMageAttributeData AttributeData = AttributeDatas->FindAttributeDataForTag(AttributeTag);
	AttributeData.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeMenuDataDelegate.Broadcast(AttributeData);
}