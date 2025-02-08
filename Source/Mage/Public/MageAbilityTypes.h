#pragma once

#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "MageAbilityTypes.generated.h"


USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr; // 上下文

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass = nullptr; // 伤害效果类

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent = nullptr; // 源技能组件

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent = nullptr; // 目标技能组件

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f; // 基础伤害
	
	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;  // 技能等级

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag(); // 伤害类型

	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f; // Debuff几率
	
	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f; // Debuff伤害
	
	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f; // Debuff持续时间
	
	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequence = 0.f; // Debuff频率

	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f; // 死亡的冲击力度

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector; // 死亡冲击力向量

	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f; // 击退几率

	UPROPERTY(BlueprintReadWrite)
	float KnockbackMagnitude = 0.f; // 击退的力

	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce = FVector::ZeroVector; //击退向量

	UPROPERTY(BlueprintReadWrite)
	bool bIsRadialDamage = false; // 是否是范围伤害

	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin = FVector::ZeroVector; // 范围伤害中心
	
	UPROPERTY(BlueprintReadWrite)
	float RadialDamageInnerRadius = 0.f; // 范围伤害内半径
	
	UPROPERTY(BlueprintReadWrite)
	float RadialDamageOuterRadius = 0.f; // 范围伤害外半径
};



/**
 * 因为原先的上下文不能满足我们传递特定变量的需求
 * 自定义的游戏效果上下文
 */

USTRUCT(BlueprintType)
struct FMageGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool GetCriticalHit() const { return bCriticalHit; }
	bool GetBlockHit() const { return bBlockHit; }
	bool GetSuccessfulDebuff() const { return bSuccessfulDebuff; }
	float GetDebuffDamage() const { return DebuffDamage; }
	float GetDebuffDuration() const { return DebuffDuration; }
	float GetDebuffFrequence() const { return DebuffFrequence; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FVector GetDeathImpulse() const { return DeathImpulse; }
	FVector GetKnockbackForce() const { return KnockbackForce; }
	bool GetIsRadialDamage() const { return bIsRadialDamage; }
	FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }
	float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }
	float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius; }
	
	void SetCriticalHit(bool bInCriticalHit) { bCriticalHit = bInCriticalHit; }
	void SetBlockHit(bool bInBlockHit) { bBlockHit = bInBlockHit; }
	void SetSuccessfulDebuff(bool bInSuccessfulDebuff) { bSuccessfulDebuff = bInSuccessfulDebuff; }
	void SetDebuffDamage(float InDebuffDamage) { DebuffDamage = InDebuffDamage; }
	void SetDebuffDuration(float InDebuffDuration) { DebuffDuration = InDebuffDuration; }
	void SetDebuffFrequence(float InDebuffFrequence) { DebuffFrequence = InDebuffFrequence; }
	void SetDamageType(const TSharedPtr<FGameplayTag>& InDamageType) { DamageType = InDamageType; }
	void SetDeathImpulse(const FVector& InDeathImpulse) { DeathImpulse = InDeathImpulse; }
	void SetKnockbackForce(const FVector& InKnockbackForce) { KnockbackForce = InKnockbackForce; }
	void SetIsRadialDamage(bool bInIsRadialDamage) { bIsRadialDamage = bInIsRadialDamage; }
	void SetRadialDamageOrigin(const FVector& InDeathImpulse) { RadialDamageOrigin = InDeathImpulse; }
	void SetRadialDamageInnerRadius(float InRadialDamageInnerRadius) { RadialDamageInnerRadius = InRadialDamageInnerRadius; }
	void SetRadialDamageOuterRadius(float InRadialDamageOuterRadius) { RadialDamageOuterRadius = InRadialDamageOuterRadius; }
	
	/**
	 * 自定义序列化，子类必须重写此函数
	 *  @param Ar 用于序列化和反序列化对象的类,它支持以二进制的形式进行加载保存和垃圾回收。
	 *  @param Map 用于处理对象在网络中映射的类。它主要用于在网络传输时跟踪对象的唯一标识,负责Object的序列化。
	 *  @param bOutSuccess 用于输出序列化的结果
	 */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
	
	/** 返回实际用于序列化的结构体，子类必须重写此函数！ */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FMageGameplayEffectContext::StaticStruct();
	}

	/** 创建此上下文的副本，用于后续修改 */
	virtual FMageGameplayEffectContext* Duplicate() const override
	{
		FMageGameplayEffectContext* NewContext = new FMageGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// 对击中结果进行深拷贝
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	
protected:

	UPROPERTY()
	bool bCriticalHit = false; // 暴击

	UPROPERTY()
	bool bBlockHit = false; // 格挡

	UPROPERTY()
	bool bSuccessfulDebuff = false; // 是否成功应用Debuff

	UPROPERTY()
	float DebuffDamage = 0.f; // Debuff伤害
	
	UPROPERTY()
	float DebuffDuration = 0.f; // 持续时间
	
	UPROPERTY()
	float DebuffFrequence = 0.f; // 频率
	
	TSharedPtr<FGameplayTag> DamageType; // 伤害类型

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector; // 造成死亡的冲击力
	
	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector; // 击退的力
	
	UPROPERTY()
	bool bIsRadialDamage = false; // 是否是范围伤害

	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector; // 范围伤害中心
	
	UPROPERTY()
	float RadialDamageInnerRadius = 0.f; // 范围伤害内半径
	
	UPROPERTY()
	float RadialDamageOuterRadius = 0.f; // 范围伤害外半径
};

// 一些C++结构的信息是不能通过模板探测出来的，就需要我们手动标记提供了。
// 在UScriptStruct::UScriptStruct这个默认构造函数中会调用PrepareCppStructOps函数，
// 引擎会检测CppStructOps（就是我们说的，包含了该结构的种种公共函数的一个变量。）的各种字段，
// 为Struct设置属性尺寸，内存对齐，以及Struct的Flag等事。

template<>
struct TStructOpsTypeTraits<FMageGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FMageGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true, // 结构体有一个NetSerialize函数，用于将其状态序列化为用于网络复制的FArchive。
		WithCopy = true // 结构可以通过其复制赋值操作符进行复制。但是对于Hit Result这种复杂类型来说，它只会复制引用。
	};
};
