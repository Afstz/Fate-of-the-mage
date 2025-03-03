// Copyright AFstz.


#include "UI/WidgetController/SkillMenuWidgetController.h"
#include "MageGameplayTags.h"
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
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
		{
			FMageAbilityData FoundData = AbilityData->FindAbilityDataForTag(AbilityTag);
			FoundData.StatusTag = StatusTag;
			AbilityDataDelegate.Broadcast(FoundData);

			// 根据选中的技能同步信息
			if (SelectedAbility.AbilityTag.MatchesTagExact(AbilityTag))
			{
				bool bSkillPointButtonEnable = false;
				bool bEquipButtonEnable = false;
				SelectedAbility.StatusTag = StatusTag;
				ShouldButtonEnables(CurrentSkillPoint, StatusTag, AbilityTag,bSkillPointButtonEnable, bEquipButtonEnable);
				FString Description;
				FString NextLevelDescription;
				GetAbilityDescFromAbilityTag(AbilityTag, Description, NextLevelDescription); // 获取技能的描述
				SphereSelectedDelegate.Broadcast(bSkillPointButtonEnable, bEquipButtonEnable, Description, NextLevelDescription); 
			}
		});
	GetMageASC()->AbilityEquippedDelegate.AddUObject(this, &ThisClass::OnAbilityEquipped);

	// PS
	GetMagePS()->SkillPointChangedDelegate.AddLambda(
		[this](const int32 InSkillPoint)
		{
			OnSkillPointChangedDelegate.Broadcast(InSkillPoint);

			// 根据选中的技能同步信息
			bool bSkillPointButtonEnable = false;
			bool bEquipButtonEnable = false;
			CurrentSkillPoint = InSkillPoint;
			ShouldButtonEnables(CurrentSkillPoint, SelectedAbility.StatusTag, SelectedAbility.AbilityTag, bSkillPointButtonEnable, bEquipButtonEnable);
			FString Description;
			FString NextLevelDescription;
			GetAbilityDescFromAbilityTag(SelectedAbility.AbilityTag, Description, NextLevelDescription); // 获取技能的描述
			SphereSelectedDelegate.Broadcast(bSkillPointButtonEnable, bEquipButtonEnable, Description, NextLevelDescription); 
		});
}

void USkillMenuWidgetController::OnSphereButtonSelected(const FGameplayTag& AbilityTag)
{
	if (bWaitingForEquip)
	{
		const FGameplayTag AbilityType = AbilityData->FindAbilityDataForTag(SelectedAbility.AbilityTag).AbilityType;
		StopWaitForEquipDelegate.Broadcast(AbilityType);
		bWaitingForEquip = false;
	}
	
	FMageGameplayTags MageGameplayTags = FMageGameplayTags::Get();

	bool bTagValid = AbilityTag.IsValid();
	bool bTagNone = AbilityTag.MatchesTagExact(MageGameplayTags.Abilities_None);
	FGameplayAbilitySpec* AbilitySpec = GetMageASC()->GetSpecFromAbilityTag(AbilityTag);
	bool bAbilitySpecNone = AbilitySpec == nullptr;

	int32 SkillPoints = GetMagePS()->GetSkillPoints();
	FGameplayTag SkillStatusTag;
	
	if (bAbilitySpecNone || !bTagValid || bTagNone) // 判断数据有效性
	{
		SkillStatusTag = MageGameplayTags.Abilities_Status_Locked; // 无效默认锁定状态
	}
	else
	{
		SkillStatusTag = GetMageASC()->GetStatusTagFromSpec(*AbilitySpec); // 有效获取实际状态
	}
	
	bool bSkillPointButtonEnable = false;
	bool bEquipButtonEnable = false;
	SelectedAbility.AbilityTag = AbilityTag;
	SelectedAbility.StatusTag = SkillStatusTag;
	ShouldButtonEnables(SkillPoints, SkillStatusTag, AbilityTag, bSkillPointButtonEnable, bEquipButtonEnable); // 根据状态和技能点判断是否启用按钮
	FString Description;
	FString NextLevelDescription;
	GetAbilityDescFromAbilityTag(AbilityTag, Description, NextLevelDescription); // 获取技能的描述
	SphereSelectedDelegate.Broadcast(bSkillPointButtonEnable, bEquipButtonEnable, Description, NextLevelDescription);
}

void USkillMenuWidgetController::ShouldButtonEnables(int32 SkillPoint, const FGameplayTag& StatusTag, const FGameplayTag& AbilityTag, bool& bShouldSkillPointEnable, bool& bShouldEquipButtonEnable)
{
	FMageGameplayTags MageGameplayTags = FMageGameplayTags::Get();

	if (AbilityTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Passive.Unupgradable"), false)) &&
		(StatusTag.MatchesTagExact(MageGameplayTags.Abilities_Status_Unlocked) ||
			StatusTag.MatchesTagExact(MageGameplayTags.Abilities_Status_Equipped)))
	{
		bShouldSkillPointEnable = false;
		bShouldEquipButtonEnable = true;
		return;
	}
	
	bShouldSkillPointEnable = false;
	bShouldEquipButtonEnable = false;
	
	if (StatusTag.MatchesTagExact(MageGameplayTags.Abilities_Status_Eligible))
	{
		if (SkillPoint > 0)
		{
			bShouldSkillPointEnable = true;
		}
	}
	else if (StatusTag.MatchesTagExact(MageGameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEquipButtonEnable = true;
		if (SkillPoint > 0)
		{
			bShouldSkillPointEnable = true;
		}
	}
	else if (StatusTag.MatchesTagExact(MageGameplayTags.Abilities_Status_Equipped))
	{
		bShouldEquipButtonEnable = true;
		if (SkillPoint > 0)
		{
			bShouldSkillPointEnable = true;
		}
	}
}

void USkillMenuWidgetController::SpendSkillPointPressed()
{
	GetMageASC()->ServerSpendSkillPoint(SelectedAbility.AbilityTag);
}

void USkillMenuWidgetController::SkillSphereDeselect()
{
	if (bWaitingForEquip)
	{
		const FGameplayTag DeselectAbilityType = AbilityData->FindAbilityDataForTag(SelectedAbility.AbilityTag).AbilityType;
		StopWaitForEquipDelegate.Broadcast(DeselectAbilityType);
		bWaitingForEquip = false;
	}
	
	SelectedAbility.AbilityTag = FGameplayTag();
	SelectedAbility.StatusTag = FGameplayTag();
	SphereSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void USkillMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag SelectedAbilityType = AbilityData->FindAbilityDataForTag(SelectedAbility.AbilityTag).AbilityType;
	WaitForEquipDelegate.Broadcast(SelectedAbilityType); // 播放对应技能类型选中动画
	bWaitingForEquip = true;

	const FGameplayTag& StatusTag = GetMageASC()->GetStatusTagFromAbilityTag(SelectedAbility.AbilityTag);
	if (StatusTag.MatchesTagExact(FMageGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedSlot = GetMageASC()->GetInputTagFromAbilityTag(SelectedAbility.AbilityTag);
	}
}

void USkillMenuWidgetController::EquipRowSpherePressed(const FGameplayTag& AbilityType, const FGameplayTag& InputTag)
{
	if (!bWaitingForEquip) return;
	
	const FGameplayTag SelectedAbilityType = AbilityData->FindAbilityDataForTag(SelectedAbility.AbilityTag).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType)) return; // 防止技能类型不一致
	
	GetMageASC()->ServerEquipAbility(SelectedAbility.AbilityTag, InputTag);
}

void USkillMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& InputTag, const FGameplayTag& PrevInputTag)
{
	if (!bWaitingForEquip) return;

	const FMageGameplayTags& MageGameplayTags = FMageGameplayTags::Get();
	// 清理插槽之前的数据
	FMageAbilityData ClearAbilityData;
	ClearAbilityData.AbilityTag = MageGameplayTags.Abilities_None;
	ClearAbilityData.StatusTag = MageGameplayTags.Abilities_Status_Unlocked;
	ClearAbilityData.AbilityInputTag = PrevInputTag;
	AbilityDataDelegate.Broadcast(ClearAbilityData);

	// 重新赋予数据
	FMageAbilityData EquippedAbilityData = AbilityData->FindAbilityDataForTag(AbilityTag);
	EquippedAbilityData.StatusTag = StatusTag;
	EquippedAbilityData.AbilityInputTag = InputTag;
	AbilityDataDelegate.Broadcast(EquippedAbilityData);

	StopWaitForEquipDelegate.Broadcast(EquippedAbilityData.AbilityType); // 停止等待装备技能,关闭选择动画
	AbilityReassignedDelegate.Broadcast(AbilityTag); // 重新指派技能成功
	SkillSphereDeselect(); // 退出装备状态
}

void USkillMenuWidgetController::ClearAllDelegate()
{
	Super::ClearAllDelegate();
	
	OnSkillPointChangedDelegate.Clear();
	SphereSelectedDelegate.Clear();
	WaitForEquipDelegate.Clear();
	StopWaitForEquipDelegate.Clear();
	AbilityReassignedDelegate.Clear();
}
