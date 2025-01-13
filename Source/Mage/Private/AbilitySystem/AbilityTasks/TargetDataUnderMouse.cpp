// Copyright AFstz.


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* TargetData = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return TargetData;
}

/**
 * 服务器释放技能本地角色执行一次
 * 
 * 客户端释放技能本地角色执行一次,服务器执行一次验证技能
 */
void UTargetDataUnderMouse::Activate()
{
	bool bLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bLocallyControlled) // 判断是否为本地角色
	{
		SendMouseTargetData();
	}
	else
	{
		FGameplayAbilitySpecHandle AbilitySpecHandle = GetAbilitySpecHandle();
		FPredictionKey PredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent->AbilityTargetDataSetDelegate(AbilitySpecHandle, PredictionKey).AddUObject(this, &ThisClass::ReplicatedTargetDataCallBack);
		// 判断客户端是否已经发来数据并缓存,有则广播数据,没有就等待数据
		const bool bTargetDataIsSet = AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(AbilitySpecHandle, PredictionKey);
		if (!bTargetDataIsSet)
		{
			SetWaitingOnRemotePlayerData(); // 设置当前任务为等待远程玩家的数据
		}
	}
}

void UTargetDataUnderMouse::SendMouseTargetData()
{
	// 创建一个预测窗口，该窗口允许客户端在不确定服务器响应的情况下，对游戏状态进行预测性更新。
	// 允许客户端向服务器发送一个新的预测键，并让服务器在相同的逻辑作用域内使用这个键。
	FScopedPredictionWindow FScopedPredictionWindow(AbilitySystemComponent.Get());
	
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorResult;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorResult);

	FGameplayAbilityTargetDataHandle TargetDataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit();
	TargetData->HitResult = CursorResult;
	TargetDataHandle.Add(TargetData); // 数据添加到句柄里指针显式转换为共享指针

	AbilitySystemComponent->ServerSetReplicatedTargetData( // 客户端向服务器发送目标数据
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		TargetDataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey
		);


	if (ShouldBroadcastAbilityTaskDelegates()) // 广播本地发来的数据
	{
		MouseTargetValidData.Broadcast(TargetDataHandle);
	}
}

void UTargetDataUnderMouse::ReplicatedTargetDataCallBack(const FGameplayAbilityTargetDataHandle& TargetDataHandle, FGameplayTag ApplicationTag)
{
	// 服务器已经接收到数据，清除缓存的数据并通知客户端已经接收
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	
	if (ShouldBroadcastAbilityTaskDelegates()) // 广播客户端发来的数据
	{
		MouseTargetValidData.Broadcast(TargetDataHandle);
	}
}