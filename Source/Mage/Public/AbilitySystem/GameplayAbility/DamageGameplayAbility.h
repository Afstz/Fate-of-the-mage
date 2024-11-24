// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
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

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

	// AI
	UFUNCTION(BlueprintPure, Category = "Mechanics")
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const; 
};
