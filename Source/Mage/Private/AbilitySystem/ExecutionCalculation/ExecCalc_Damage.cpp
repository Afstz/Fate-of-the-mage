// Copyright AFstz.


#include "AbilitySystem/ExecutionCalculation/ExecCalc_Damage.h"
#include "MageGameplayTags.h"
#include "AbilitySystem/MageAbilitySystemLibrary.h"
#include "AbilitySystem/MageAttributeSet.h"
#include "Interface/CombatInterface.h"


struct	FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor); // 声明捕获结构体
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistence);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicalResistence);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistence);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistence);
	FDamageStatics()
	{
		// Params: 1.属性集 2.属性名 3.目标还是自身 4.是否设置快照(true为创建时获取属性，false为应用时获取属性)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMageAttributeSet, Armor, Target, false); // 实现捕获结构体
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMageAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMageAttributeSet, ArmorPenetration, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMageAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMageAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMageAttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMageAttributeSet, PhysicalResistence, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMageAttributeSet, MagicalResistence, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMageAttributeSet, FireResistence, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMageAttributeSet, LightningResistence, Target, false);
	}
};

static const FDamageStatics& GetDamageStatics()
{
	static FDamageStatics DamageStatics; // 静态变量只初始化一次
	return DamageStatics;
}


UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(GetDamageStatics().ArmorDef); // 把要捕获的属性定义添加到捕获数组里
	RelevantAttributesToCapture.Add(GetDamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().PhysicalResistenceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().MagicalResistenceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().FireResistenceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().LightningResistenceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	int32 SourceAvatarLevel = 1; 
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourceAvatarLevel = ICombatInterface::Execute_GetCharacterLevel(SourceAvatar);
	}
	int32 TargetAvatarLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetAvatarLevel = ICombatInterface::Execute_GetCharacterLevel(TargetAvatar);
	}
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	FAggregatorEvaluateParameters AggregatorParameters;
	AggregatorParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	AggregatorParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> DamageTypesToDef; // 方便计算抗性
	DamageTypesToDef.Add(FMageGameplayTags::Get().Attributes_Secondary_PhysicalResistence, GetDamageStatics().PhysicalResistenceDef);
	DamageTypesToDef.Add(FMageGameplayTags::Get().Attributes_Secondary_MagicalResistence, GetDamageStatics().MagicalResistenceDef);
	DamageTypesToDef.Add(FMageGameplayTags::Get().Attributes_Secondary_FireResistence, GetDamageStatics().FireResistenceDef);
	DamageTypesToDef.Add(FMageGameplayTags::Get().Attributes_Secondary_LightningResistence, GetDamageStatics().LightningResistenceDef);
	
	// 通过标签获取SetByCaller设置的伤害值同时计算抗性伤害
	float TotalDamage = 0.f;
	for (const auto& [DamageType, DamageTypeResistence] : FMageGameplayTags::Get().DamageTypesToResistence)
	{
		checkf(DamageTypesToDef.Contains(DamageTypeResistence), TEXT("Not Find [%s] in DamageTypesToDef"), *DamageTypeResistence.ToString());
		const FGameplayEffectAttributeCaptureDefinition ResistenceDef = DamageTypesToDef[DamageTypeResistence];
		float ResistancePercentage = 0.f; // 当前敌方属性抗性
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ResistenceDef, AggregatorParameters, ResistancePercentage);
		ResistancePercentage = FMath::Clamp(ResistancePercentage, 0.f, 100.f); // 确保抗性值在百分比范围内

		// WarnIfNotFound:false 没找到不用打印日志
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageType, false, 0); // 当前类型属性伤害
		// 按敌方抗性值百分比降低伤害
		DamageTypeValue *= (100 - ResistancePercentage) / 100;
		
		TotalDamage += DamageTypeValue; // 累加计算后的总伤害
	}
	
	// 获取伤害计算相关系数
	const UCharacterClassData* CharacterClassData = UMageAbilitySystemLibrary::GetCharacterClassData(SourceAvatar);
	const UCurveTable* DamageCoefficientTable = CharacterClassData->DamageCoefficientTables;

	// 1. 计算己方是否暴击
	float SourceCriticalHitChance = 0.f; // 己方暴击几率
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FDamageStatics().CriticalHitChanceDef, AggregatorParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Clamp(SourceCriticalHitChance, 0.f, 100.f);
	float SourceCriticalHitDamage = 0.f; // 己方暴击伤害
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FDamageStatics().CriticalHitDamageDef, AggregatorParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max(0.f, SourceCriticalHitDamage);
	float TargetCriticalHitRes = 0.f; // 敌方暴击抗性
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FDamageStatics().CriticalHitResistanceDef, AggregatorParameters, TargetCriticalHitRes);
	TargetCriticalHitRes = FMath::Max(0.f, TargetCriticalHitRes);
	
	const FRealCurve* CriticalHitResistanceCurve = DamageCoefficientTable->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoeff = CriticalHitResistanceCurve->Eval(TargetAvatarLevel);

	// 己方爆率减去敌方抗性
	float EffectiveCriticalHit = SourceCriticalHitChance - TargetCriticalHitRes * CriticalHitResistanceCoeff;
	const bool bCriticalHit = EffectiveCriticalHit >= FMath::RandRange(1 , 100); // 判断是否暴击
	TotalDamage = bCriticalHit ? TotalDamage * 2.f + SourceCriticalHitDamage : TotalDamage; // 暴击成功双倍并加上爆伤

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	UMageAbilitySystemLibrary::SetCriticalHit(EffectContextHandle, bCriticalHit); // 设置上下文
	
	// 2. 计算敌方是否格挡
	float TargetBlockChance = 0.f; // 敌方格挡几率
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FDamageStatics().BlockChanceDef, AggregatorParameters, TargetBlockChance);
	TargetBlockChance = FMath::Clamp<float>(TargetBlockChance, 0.f, 100.f);
	const bool bBlocked = TargetBlockChance >= FMath::RandRange(1, 100); // 判断是否格挡
	TotalDamage = bBlocked ? TotalDamage / 2.f : TotalDamage; // 格挡成功伤害减半
	
	UMageAbilitySystemLibrary::SetBlockHit(EffectContextHandle, bBlocked); // 设置上下文

	// 3. 计算敌方护甲和己方护甲穿透
	float TargetArmor = 0.f; // 敌方护甲
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FDamageStatics().ArmorDef, AggregatorParameters, TargetArmor);
	TargetArmor = FMath::Max(0, TargetArmor);
	float SourceArmorPenetration = 0.f; // 己方护甲穿透
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FDamageStatics().ArmorPenetrationDef, AggregatorParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max(0, SourceArmorPenetration);
	
	const FRealCurve* ArmorPenetrationCurve = DamageCoefficientTable->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoeff = ArmorPenetrationCurve->Eval(SourceAvatarLevel);
	// 根据己方护甲穿透忽略一定百分比的护甲, 护甲穿透按一定的比率有效降低敌方护甲。
	const float EffectiveTargetArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoeff) / 100;
	
	const FRealCurve* ArmorCurve = DamageCoefficientTable->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoeff = ArmorCurve->Eval(TargetAvatarLevel);
	// 根据敌方护甲忽略一定百分比的伤害, 敌方护甲按一定的比率有效降低伤害。
	TotalDamage *= (100 - EffectiveTargetArmor * EffectiveArmorCoeff) / 100;

	// 输出计算
 	FGameplayModifierEvaluatedData EvaluatedData = FGameplayModifierEvaluatedData(UMageAttributeSet::GetReceivedDamageAttribute(), EGameplayModOp::Additive, TotalDamage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}