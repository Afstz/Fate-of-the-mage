// Copyright AFstz.

#include "AbilitySystem/GameplayAbility/DamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interface/EnemyInterface.h"

void UDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	const FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);

	const float DamageMagnitude = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, DamageMagnitude);
	
	// 对敌方造成伤害
	UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor)->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data.Get());
}

FTaggedMontage UDamageGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (TaggedMontages.Num() > 0)
	{
		const int32 RandomIndex = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[RandomIndex];
	}
	
	return FTaggedMontage();
}

float UDamageGameplayAbility::GetAbilityDamage(int32 InAbilityLevel)
{
	float CausedDamage = Damage.GetValueAtLevel(InAbilityLevel);
	
	return CausedDamage;
}

FDamageEffectParams UDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor) const
{
	FDamageEffectParams DamageEffectParams;
	
	DamageEffectParams.WorldContextObject = GetAvatarActorFromActorInfo();
	DamageEffectParams.DamageEffectClass = DamageEffectClass;
	DamageEffectParams.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	DamageEffectParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	DamageEffectParams.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	DamageEffectParams.DamageType = DamageType;
	DamageEffectParams.AbilityLevel = GetAbilityLevel();
	DamageEffectParams.DebuffChance = DebuffChance;
	DamageEffectParams.DebuffDamage = DebuffDamage;
	DamageEffectParams.DebuffDuration = DebuffDuration;
	DamageEffectParams.DebuffFrequence = DebuffFrequence;
	DamageEffectParams.DeathImpulseMagnitude = DeathImpulseMagnitude;
	DamageEffectParams.KnockbackChance = KnockbackChance;
	DamageEffectParams.KnockbackMagnitude = KnockbackMagnitude;
	if (IsValid(TargetActor))
	{
		FRotator KnockbackForceRotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		KnockbackForceRotation.Pitch = 45.f;
		const FVector ToTarget = KnockbackForceRotation.Vector();
		DamageEffectParams.DeathImpulse = ToTarget * DeathImpulseMagnitude; // 死亡的冲击力
		if (bool bKnockbackSuccessful = KnockbackChance >= FMath::RandRange(1, 100))
		{
			DamageEffectParams.KnockbackForce = ToTarget * KnockbackMagnitude; // 击退的力
		}
	}

	return DamageEffectParams;
}