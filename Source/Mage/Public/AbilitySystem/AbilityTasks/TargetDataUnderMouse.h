// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, Data);

/**
 *	获取鼠标光标下位置的技能任务
 */
UCLASS()
class MAGE_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", DisplayName= "TargetDataUnderMouse", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UTargetDataUnderMouse* CreateTargetDtatUnderMouse(UGameplayAbility* OwningAbility); // 工厂函数创建技能任务
	
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature MouseTargetValidData; // 暴露给蓝图广播数据
	
protected:
	virtual void Activate() override;
private:
	void SendMouseTargetData(); // 客户端发送目标数据到服务器

	// 接收客户端发来的数据的回调函数
	void TargetDataReplicatedCallBack(const FGameplayAbilityTargetDataHandle& TargetDataHandle, FGameplayTag ApplicationTag);
};
