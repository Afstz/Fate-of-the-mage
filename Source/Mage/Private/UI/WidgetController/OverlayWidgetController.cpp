// Copyright AFstz.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/MageAbilitySystemComponent.h"
#include "AbilitySystem/MageAttributeSet.h"
#include "AbilitySystem/Data/AbilityData.h"
#include "AbilitySystem/Data/LevelUpData.h"
#include "Player/MagePlayerState.h"

void UOverlayWidgetController::BroadcastInitialValue()
{
	UMageAttributeSet* MageAttributeSet = CastChecked<UMageAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(MageAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(MageAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(MageAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(MageAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	UMageAttributeSet* MageAttributeSet = CastChecked<UMageAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate( // 根据输入的Attribute绑定回调函数
		MageAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		MageAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		MageAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		MageAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);

	if (UMageAbilitySystemComponent* MageASC = Cast<UMageAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (MageASC->bStartupAbilitiesGiven) // 技能初始化完毕就直接执行否则绑定委托
		{
			OnInitializeStartupAbilities(MageASC);
		}
		else
		{
			MageASC->AbilitiesGivenDelegate.AddUObject(this, &ThisClass::OnInitializeStartupAbilities);
		}
		
		MageASC->EffectAssetTagsDelegate.AddLambda(
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

	AMagePlayerState* MagePS = CastChecked<AMagePlayerState>(PlayerState);
	MagePS->XPChangedDelegate.AddUObject(this, &ThisClass::OnXPChanged);
	MagePS->LevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnLevelChangedDelegate.Broadcast(NewLevel);
		});
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UMageAbilitySystemComponent* MageASC)
{
	if (!MageASC->bStartupAbilitiesGiven) return;

	FForeachAbilitiesSignature ForeachDelegate; // 单播委托
	ForeachDelegate.BindLambda([this, MageASC](const FGameplayAbilitySpec& AbilitySpec)
	{
		FMageAbilityData FoundData = AbilityData->FindAbilityDataForTag(MageASC->GetAbilityTagFromSpec(AbilitySpec));
		FoundData.AbilityInputTag = MageASC->GetInputTagFromSpec(AbilitySpec); // 获取到技能的输入标签
		AbilityDataDelegate.Broadcast(FoundData);
	});
	// 遍历技能并执行委托回调
	MageASC->ForeachAbilitiesExecute(ForeachDelegate);
}

void UOverlayWidgetController::OnXPChanged(const int32 NewXP) const
{
	const AMagePlayerState* MagePS = CastChecked<AMagePlayerState>(PlayerState);
	const ULevelUpData* LevelUpData = MagePS->LevelUpData;
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
