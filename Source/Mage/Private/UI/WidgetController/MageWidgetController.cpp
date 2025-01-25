// Copyright AFstz.


#include "UI/WidgetController/MageWidgetController.h"
#include "MageGameplayTags.h"
#include "AbilitySystem/MageAbilitySystemComponent.h"
#include "AbilitySystem/MageAttributeSet.h"
#include "AbilitySystem/Data/AbilityData.h"
#include "AbilitySystem/GameplayAbility/MageGameplayAbility.h"
#include "Player/MagePlayerController.h"
#include "Player/MagePlayerState.h"

void UMageWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& InWidgetControllerParams)
{
	PlayerController = InWidgetControllerParams.PlayerController;
	PlayerState = InWidgetControllerParams.PlayerState;
	AbilitySystemComponent = InWidgetControllerParams.AbilitySystemComponent;
	AttributeSet = InWidgetControllerParams.AttributeSet;
	
}

void UMageWidgetController::BroadcastInitialValue()
{
}

void UMageWidgetController::BindCallbacksToDependencies()
{
}

AMagePlayerController* UMageWidgetController::GetMagePC()
{
	if (MagePlayerController == nullptr)
	{
		MagePlayerController = Cast<AMagePlayerController>(PlayerController);
	}
	return MagePlayerController;
}

AMagePlayerState* UMageWidgetController::GetMagePS()
{
	if (MagePlayerState == nullptr)
	{
		MagePlayerState = Cast<AMagePlayerState>(PlayerState);
	}
	return MagePlayerState;
}

UMageAbilitySystemComponent* UMageWidgetController::GetMageASC()
{
	if (MageAbilitySystemComponent == nullptr)
	{
		MageAbilitySystemComponent = Cast<UMageAbilitySystemComponent>(AbilitySystemComponent);
	}
	return MageAbilitySystemComponent;
}

UMageAttributeSet* UMageWidgetController::GetMageAS()
{
	if (MageAttributeSet == nullptr)
	{
		MageAttributeSet = Cast<UMageAttributeSet>(AttributeSet);
	}
	return MageAttributeSet;
}

void UMageWidgetController::BroadcastAbilityData()
{
	if (!GetMageASC()->bStartupAbilitiesGiven) return;

	FForeachAbilitiesSignature ForeachDelegate; // 单播委托
	ForeachDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FMageAbilityData FoundData = AbilityData->FindAbilityDataForTag(MageAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec), false);
		FoundData.AbilityInputTag = MageAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec); // 获取到技能的输入标签
		FoundData.StatusTag = MageAbilitySystemComponent->GetStatusTagFromSpec(AbilitySpec); // 获取到技能的状态标签
		AbilityDataDelegate.Broadcast(FoundData);
	});
	
	// 遍历技能并执行委托回调
	GetMageASC()->ForeachAbilitiesExecute(ForeachDelegate);
}

bool UMageWidgetController::GetAbilityDescFromAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetMageASC()->GetSpecFromAbilityTag(AbilityTag))
	{
		// 已有的技能
		if (UMageGameplayAbility* MageGameplayAbility = Cast<UMageGameplayAbility>(AbilitySpec->Ability))
		{
			OutDescription = MageGameplayAbility->GetDescription(AbilitySpec->Level);
			OutNextLevelDescription = MageGameplayAbility->GetNextLevelDescription(AbilitySpec->Level + 1);
			return true;
		}
	}
	// 未解锁的技能
	if (!AbilityTag.IsValid() || AbilityTag.MatchesTagExact(FMageGameplayTags::Get().Abilities_None))
	{
		// 无效技能
		OutDescription = FString();
	}
	else
	{
		// 有效技能
		FMageAbilityData Data = AbilityData->FindAbilityDataForTag(AbilityTag, false);
		OutDescription = UMageGameplayAbility::GetLockedDescription(Data.AbilityLevelRequirement); 
	}
	OutNextLevelDescription = FString();
	return false;
}

void UMageWidgetController::ClearAllDelegate()
{
	AbilityDataDelegate.Clear();
}
