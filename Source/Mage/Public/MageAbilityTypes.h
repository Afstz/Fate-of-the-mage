#pragma once

#include "GameplayEffectTypes.h"
#include "MageAbilityTypes.generated.h"

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
	void SetCriticalHit(bool bInIsCriticalHit) { bCriticalHit = bInIsCriticalHit; }
	void SetBlockHit(bool bInIsBlockHit) { bBlockHit = bInIsBlockHit; }
	
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
