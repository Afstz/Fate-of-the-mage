// Copyright AFstz.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/MageAbilitySystemComponent.h"
#include "AbilitySystem/MageAttributeSet.h"
#include "AbilitySystem/Data/AbilityData.h"
#include "AbilitySystem/Data/LevelUpData.h"
#include "Player/MagePlayerState.h"

void UOverlayWidgetController::BroadcastInitialValue()
{
	OnHealthChanged.Broadcast(GetMageAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetMageAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetMageAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetMageAS()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	// AttributeSet 回调函数绑定

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( // 根据输入的Attribute绑定回调函数
		GetMageAS()->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetMageAS()->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetMageAS()->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetMageAS()->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);

	if (GetMageASC())
	{
		if (GetMageASC()->bStartupAbilitiesGiven) // 技能初始化完毕就直接执行否则绑定委托
		{
			BroadcastAbilityData();
		}
		else
		{
			GetMageASC()->AbilitiesGivenDelegate.AddUObject(this, &ThisClass::BroadcastAbilityData);
		}
		
		GetMageASC()->EffectAssetTagsDelegate.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				// 获取 Message 标签
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				// MatchesTag : "A.1".MatchesTag("A") will return True, "A".MatchesTag("A.1") will return False
				if (Tag.MatchesTag(MessageTag)) // 判断是否是Message 子Tag
				{
					FMessageWdigetData* TableRow = GetDataTableRowByTag<FMessageWdigetData>(MessageDataTable, Tag);
					MessageWidgetDelegate.Broadcast(*TableRow); // 广播相匹配的对应表行
				}
			}
		});
	}
	
	// PlayerState 回调函数绑定
	GetMagePS()->XPChangedDelegate.AddUObject(this, &ThisClass::OnXPChanged);
	GetMagePS()->LevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnLevelChangedDelegate.Broadcast(NewLevel);
		});
}

void UOverlayWidgetController::OnXPChanged(const int32 NewXP)
{
	// 计算当前经验值在升级区间的百分比
	
	const ULevelUpData* LevelUpData = GetMagePS()->LevelUpData;
	checkf(LevelUpData, TEXT("Not Find LevelUpData In PlayerState."))
	
	int32 CurrentLevel = LevelUpData->FindLevelForXP(NewXP);
	int32 MaxLevel = LevelUpData->LevelUpDatas.Num();

	if (CurrentLevel <= MaxLevel && CurrentLevel > 0)
	{
		int32 CurrLevelUpRequirement = LevelUpData->LevelUpDatas[CurrentLevel].LevelUpRequirement;
		int32 PreLevelUpRequirement = LevelUpData->LevelUpDatas[CurrentLevel - 1].LevelUpRequirement;
		
		int32 DeltaLevelUpRequirement = CurrLevelUpRequirement - PreLevelUpRequirement; // 当前等级经验区间
		int32 XPForThisLevel = NewXP - PreLevelUpRequirement; // 当前区间已有的经验
		float LevelUpPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelUpRequirement);

		OnXPPercentChangedDelegate.Broadcast(LevelUpPercent);
	}
}
