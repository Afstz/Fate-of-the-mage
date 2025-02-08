// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "MageAbilityTypes.h"
#include "AbilitySystem/GameplayAbility/MageGameplayAbility.h"
#include "DamageGameplayAbility.generated.h"

struct FTaggedMontage;
/**
 *  伤害玩法技能类
 */
UCLASS()
class MAGE_API UDamageGameplayAbility : public UMageGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void CauseDamage(AActor* TargetActor);
	
	// 获取技能伤害
	UFUNCTION(BlueprintPure)
	float GetAbilityDamage(int32 InAbilityLevel) const; 
protected:
	// Damage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// 伤害类型
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageType;

	// 造成的伤害
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat Damage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffChance = 10.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDamage = 5.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDuration = 3.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffFrequence = 1.f;

	// 造成死亡的冲击力(>0 必定)
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DeathImpulseMagnitude = 10.f; 

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float KnockbackChance = 0.f; 

	// 击退的力(基于概率)
	UPROPERTY(EditDefaultsOnly, Category = "Damage") 
	float KnockbackMagnitude = 0.f;

	// 是否是范围伤害
	UPROPERTY(EditDefaultsOnly, Category = "Damage") 
	bool bIsRadialDamage = false; 

	// 范围伤害内半径
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage") 
	float RadialDamageInnerRadius = 0.f; 

	// 范围伤害外半径
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage") 
	float RadialDamageOuterRadius = 0.f; 

	UFUNCTION(BlueprintPure, Category = "Damage")
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(
		AActor* TargetActor = nullptr,
		FVector InRadialDamageOrigin = FVector::ZeroVector,
		bool bOverridePitch = false,
		float InPitchOverride = 0.f) const;
	
	// AI
	UFUNCTION(BlueprintPure, Category = "Mechanics")
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;

private:
	
};
