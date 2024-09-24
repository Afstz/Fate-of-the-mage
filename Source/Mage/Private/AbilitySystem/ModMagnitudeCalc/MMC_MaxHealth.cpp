// Copyright AFstz.


#include "AbilitySystem/ModMagnitudeCalc/MMC_MaxHealth.h"

#include "AbilitySystem/MageAttributeSet.h"
#include "Interface/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UMageAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 收集 Spec 的 Source 和 Target 的Tags
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// 存储关于源和目标标签的聚合信息
	// 这个参数会传递给后面的计算函数，以便在计算属性时考虑这些标签。
	FAggregatorEvaluateParameters AggregatorParameters;
	AggregatorParameters.SourceTags = SourceTags;
	AggregatorParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, AggregatorParameters, Vigor); // 根据捕获的属性获取值
	Vigor = FMath::Max(Vigor, 0.f); // 防止小于零

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	int32 Level = CombatInterface->GetCharacterLevel();

	 return (100 + Vigor * 2) + 10 * Level; // 基于 Vigor 和 Level
}