#include "MageAbilityTypes.h"

bool FMageGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	uint32 RepBits = 0;
	if (Ar.IsSaving()) // 是否在序列化
	{
		// 判断变量是否要序列化，通过或运算添加到RepBits里
		if (bReplicateInstigator && Instigator.IsValid()) 
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid() )
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
		if (bCriticalHit)
		{
			RepBits |= 1 << 7;
		}
		if (bBlockHit)
		{
			RepBits |= 1 << 8;
		}
		if (bSuccessfulDebuff)
		{
			RepBits |= 1 << 9;
		}
		if (DebuffDamage)
		{
			RepBits |= 1 << 10;
		}
		if (DebuffDuration)
		{
			RepBits |= 1 << 11;
		}
		if (DebuffFrequence)
		{
			RepBits |= 1 << 12;
		}
		if (DamageType.IsValid())
		{
			RepBits |= 1 << 13;
		}
		if (!DeathImpulse.IsZero())
		{
			RepBits |= 1 << 14;
		}
		if (!KnockbackForce.IsZero())
		{
			RepBits |= 1 << 15;
		}
		if (bIsRadialDamage)
		{
			RepBits |= 1 << 16;
			
			if (RadialDamageOrigin.IsZero())
			{
				RepBits |= 1 << 17;
			}
			if (RadialDamageInnerRadius > 0.f)
			{
				RepBits |= 1 << 18;
			}
			if (RadialDamageOuterRadius > 0.f)
			{
				RepBits |= 1 << 19;
			}
		}
	}

	// 将RepBits的值序列化，使用9位长度保存
	Ar.SerializeBits(&RepBits, 20);

	// 判断相对于的变量需要序列化或反序列化(重载过后的左移运算符会根据上下文切换是序列化还是反序列化)
	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}
	if (RepBits & (1 << 7))
	{
		Ar << bCriticalHit;
	}
	if (RepBits & (1 << 8))
	{
		Ar << bBlockHit;
	}
	if (RepBits & (1 << 9))
	{
		Ar << bSuccessfulDebuff;
	}
	if (RepBits & (1 << 10))
	{
		Ar << DebuffDamage;
	}
	if (RepBits & (1 << 11))
	{
		Ar << DebuffDuration;
	}
	if (RepBits & (1 << 12))
	{
		Ar << DebuffFrequence;
	}
	if (RepBits & (1 << 13))
	{
		if (Ar.IsLoading())
		{
			if (!DamageType.IsValid())
			{
				DamageType = TSharedPtr<FGameplayTag>(new FGameplayTag());
			}
		}
		DamageType->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 14))
	{
		DeathImpulse.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 15))
	{
		KnockbackForce.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 16))
	{
		Ar << bIsRadialDamage;
		
		if (RepBits & (1 << 17))
		{
			RadialDamageOrigin.NetSerialize(Ar, Map, bOutSuccess);
		}
		if (RepBits & (1 << 18))
		{
			Ar << RadialDamageInnerRadius;
		}
		if (RepBits & (1 << 19))
		{
			Ar << RadialDamageOuterRadius;
		}
	}

	//如果是反序列化时，需要调用对ASC进行初始化
	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}	
	
	bOutSuccess = true;
	return true;
}