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

	virtual FString GetDescription(int32 AbilityLevel) override;
	virtual FString GetNextLevelDescription(int32 AbilityLevel) override;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Passive")
	TSubclassOf<UGameplayEffect> PassiveEffectClass;
	UFUNCTION(BlueprintCallable)
	void ApplyPassiveGameplayEffect(); // 应用被动技能Effect
	UFUNCTION(BlueprintCallable)
	void RemovePassiveGameplayEffect();

	// Description
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanUpgrade = true;
	UPROPERTY(EditDefaultsOnly)
	FString AbilityName = FString();
	UPROPERTY(EditDefaultsOnly)
	FString AbilityDescription = FString();
private:
	FActiveGameplayEffectHandle ActiveEffectHandle = FActiveGameplayEffectHandle();
};
