// Copyright AFstz.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/MageAbilitySystemComponent.h"
#include "AbilitySystem/MageAttributeSet.h"
#include "AbilitySystem/Data/AttributeData.h"
#include "Player/MagePlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValue()
{
	// AttributeSet 
	UMageAttributeSet* AS = Cast<UMageAttributeSet>(AttributeSet);

	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeData(Pair.Key, Pair.Value);
	}
	
	// PlayerState
	AMagePlayerState* PS = CastChecked<AMagePlayerState>(PlayerState);
	OnAttributePointChangedDelegate.Broadcast(PS->GetAttributePoints());
	OnSkillPointChangedDelegate.Broadcast(PS->GetSkillPoints());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	// AttributeSet 
	UMageAttributeSet* AS = CastChecked<UMageAttributeSet>(AttributeSet);
	
	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeData(Pair.Key, Pair.Value);
			}
		);
	}

	// PlayerState
	AMagePlayerState* PS = CastChecked<AMagePlayerState>(PlayerState);
	PS->AttributePointChangedDelegate.AddLambda(
	[this](const int32& NewAttributePoint)
		{
			OnAttributePointChangedDelegate.Broadcast(NewAttributePoint);
		});
	PS->SkillPointChangedDelegate.AddLambda(
		[this](const int32& NewSkillPoint)
		{
			OnSkillPointChangedDelegate.Broadcast(NewSkillPoint);
		});
}

void UAttributeMenuWidgetController::BroadcastAttributeData(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	check(AttributeData);
	FMageAttributeData FoundAttributeData = AttributeData->FindAttributeDataForTag(AttributeTag);
	FoundAttributeData.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeMenuDataDelegate.Broadcast(FoundAttributeData);
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UMageAbilitySystemComponent* ASC = CastChecked<UMageAbilitySystemComponent>(AbilitySystemComponent);
	ASC->UpgradeAttribute(AttributeTag);
}
