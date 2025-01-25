// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "MageGameplayAbility.h"
#include "PassiveAbility.generated.h"

/**
 *  被动技能
 */
UCLASS()
class MAGE_API UPassiveAbility : public UMageGameplayAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void OnAbilityDeactivate(const FGameplayTag& AbilityTag);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Passive")
	TSubclassOf<UGameplayEffect> PassiveEffectClass;
	UFUNCTION(BlueprintCallable)
	void ApplyPassiveGameplayEffect(); // 应用被动技能Effect
	UFUNCTION(BlueprintCallable)
	void RemovePassiveGameplayEffect();
private:
	FActiveGameplayEffectHandle ActiveEffectHandle = FActiveGameplayEffectHandle();
};
