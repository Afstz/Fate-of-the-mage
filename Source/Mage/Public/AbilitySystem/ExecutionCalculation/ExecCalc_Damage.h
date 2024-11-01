// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

/**
 *  它和GameplayModMagnitudeCalculation很像，但是可以一次性修改多个属性。
 *  它不是只返回一个值，而是直接在函数内部对属性进行了修改。
 */
UCLASS()
class MAGE_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UExecCalc_Damage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
