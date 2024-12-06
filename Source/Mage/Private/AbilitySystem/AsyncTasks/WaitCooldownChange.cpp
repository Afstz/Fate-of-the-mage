// Copyright AFstz.


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"
#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::WaitCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag)
{
	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
	WaitCooldownChange->ASC = AbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;

	if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid()) // 判断数据有效性
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}

	// 绑定技能结束监听事件
	AbilitySystemComponent->RegisterGameplayTagEvent(
		InCooldownTag,
		EGameplayTagEventType::NewOrRemoved).AddUObject(
			WaitCooldownChange,
			&ThisClass::OnCooldownTagChanged);

	// 绑定技能开始监听事件
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange, &ThisClass::OnActiveEffectAdded);
	
	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	if (!IsValid(ASC)) return;

	ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	ASC->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);

	SetReadyToDestroy(); // 通知引擎该对象已经准备好被销毁
	MarkAsGarbage(); // 标记此实例为垃圾
}

void UWaitCooldownChange::OnCooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0) // 冷却阶段已经结束
	{
		CooldownEnd.Broadcast(0.f);
	}
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer AssetTags;
	EffectSpec.GetAllAssetTags(AssetTags); // 获取GE自身标签
	FGameplayTagContainer GrantedTags;
	EffectSpec.GetAllGrantedTags(GrantedTags); // 获取GE赋予Actor的标签

	// 判断GE拥有冷却标签
	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		// 创建一个查询对象, 找到所有对应拥有赋予标签的GE
		FGameplayEffectQuery EffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		// 根据查询到的Duration GEs得到所有的剩余时间
		TArray<float> RemainingTimes = ASC->GetActiveEffectsTimeRemaining(EffectQuery);
		if (RemainingTimes.Num() > 0)
		{
			float MaxTimeRemaining = 0; // 保证有效性，获取到数组里最长的剩余时间
			for (int i = 0; i < RemainingTimes.Num(); i++)
			{
				if (RemainingTimes[i] > MaxTimeRemaining)
				{
					MaxTimeRemaining = RemainingTimes[i];
				}
			}
			CooldownStart.Broadcast(MaxTimeRemaining);
		}
	}
}