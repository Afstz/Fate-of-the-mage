// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "MageInputData.h"
#include "MageInputComponent.generated.h"

/**
 * 自定义的增强输入类，便于绑定玩法能力事件
 */
UCLASS()
class MAGE_API UMageInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	/**
	 * 从MageInputData获取技能相关Tag,对所有技能进行事件绑定
	 * @param Object 对象
	 * @param PressedFunc 按下按键函数指针
	 * @param HeldFunc 按住按键函数指针
	 * @param ReleasedFunc 释放按键函数指针
	 */
	template<class UserClass, typename PressedFuncPtr, typename HeldFuncPtr, typename ReleasedFuncPtr>
	void BindAbilityActions(const UMageInputData* InputData, UserClass* Object,
		PressedFuncPtr PressedFunc, HeldFuncPtr HeldFunc, ReleasedFuncPtr ReleasedFunc);
	
};

template <class UserClass, typename PressedFuncPtr, typename HeldFuncPtr, typename ReleasedFuncPtr>
void UMageInputComponent::BindAbilityActions(const UMageInputData* InputData, UserClass* Object,
	PressedFuncPtr PressedFunc, HeldFuncPtr HeldFunc, ReleasedFuncPtr ReleasedFunc)
{
	check(InputData);

	for (const FMageInputActionData& InputActionData : InputData->AbilityInputActions)
	{
		if (InputActionData.InputAction && InputActionData.InputGameplayTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(InputActionData.InputAction, ETriggerEvent::Started, Object, PressedFunc, InputActionData.InputGameplayTag);
			}
			if (HeldFunc)
			{
				BindAction(InputActionData.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, InputActionData.InputGameplayTag);
			}
			if (ReleasedFunc)
			{
				BindAction(InputActionData.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputActionData.InputGameplayTag);
			}
		}
	}
}
