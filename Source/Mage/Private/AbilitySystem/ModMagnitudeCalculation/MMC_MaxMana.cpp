// Copyright AFstz.


#include "AbilitySystem/ModMagnitudeCalculation/MMC_MaxMana.h"

#include "AbilitySystem/MageAttributeSet.h"
#include "Interface/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDef.AttributeToCapture = UMageAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 收集 Spec 的 Source 和 Target 的Tags
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// 存储关于源和目标标签的聚合信息
	// 这个参数会传递给后面的计算函数，以便在计算属性时考虑这些标签。
	FAggregatorEvaluateParameters AggregatorParameters;
	AggregatorParameters.SourceTags = SourceTags;
	AggregatorParameters.TargetTags = TargetTags;

	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, AggregatorParameters, Intelligence);
	Intelligence = FMath::Max(Intelligence, 0.f);

	int32 Level = 1;
	if (IsValid(Spec.GetContext().GetSourceObject()) && Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		Level = ICombatInterface::Execute_GetCharacterLevel(Spec.GetContext().GetSourceObject());
	}

	return (60 + Intelligence * 2) + 10 * Level; // 基于 Intelligence 和 Level
}