// Copyright AFstz.


#include "UI/WidgetController/SkillMenuWidgetController.h"

#include "AbilitySystem/MageAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityData.h"
#include "Player/MagePlayerState.h"

void USkillMenuWidgetController::BroadcastInitialValue()
{
	BroadcastAbilityData();

	OnSkillPointChangedDelegate.Broadcast(GetMagePS()->GetSkillPoints());
}

void USkillMenuWidgetController::BindCallbacksToDependencies()
{
	check(AbilityData)

	// ASC
	GetMageASC()->AbilityStatusChanged.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
		{
			FMageAbilityData FoundData = AbilityData->FindAbilityDataForTag(AbilityTag);
			FoundData.StatusTag = StatusTag;
			AbilityDataDelegate.Broadcast(FoundData);
		});

	// PS
	GetMagePS()->SkillPointChangedDelegate.AddLambda(
		[this](const int32 InSkillPoint)
		{
			OnSkillPointChangedDelegate.Broadcast(InSkillPoint);
		});
}