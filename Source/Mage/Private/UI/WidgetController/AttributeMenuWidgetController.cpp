// Copyright AFstz.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/MageAbilitySystemComponent.h"
#include "AbilitySystem/MageAttributeSet.h"
#include "AbilitySystem/Data/AttributeData.h"
#include "Player/MagePlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValue()
{
	// AttributeSet
	
	for (auto& Pair : GetMageAS()->TagsToAttributes)
	{
		BroadcastAttributeData(Pair.Key, Pair.Value);
	}
	
	// PlayerState
	OnAttributePointChangedDelegate.Broadcast(GetMagePS()->GetAttributePoints());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	// AttributeSet 
	check(AttributeData);
	for (auto& Pair : GetMageAS()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeData(Pair.Key, Pair.Value);
			}
		);
	}

	// PlayerState

	GetMagePS()->AttributePointChangedDelegate.AddLambda(
	[this](const int32& NewAttributePoint)
		{
			OnAttributePointChangedDelegate.Broadcast(NewAttributePoint);
		});
}

void UAttributeMenuWidgetController::BroadcastAttributeData(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FMageAttributeData FoundAttributeData = AttributeData->FindAttributeDataForTag(AttributeTag);
	FoundAttributeData.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeMenuDataDelegate.Broadcast(FoundAttributeData);
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetMageASC()->UpgradeAttribute(AttributeTag);
}
