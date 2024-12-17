// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MageAbilitySystemComponent.generated.h"

class UMageAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsSignature, const FGameplayTagContainer& /* Asset Tags */);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGivenSignature, UMageAbilitySystemComponent*); // 技能初始化委托
DECLARE_DELEGATE_OneParam(FForeachAbilitiesSignature, const FGameplayAbilitySpec&); // 遍历技能单播委托
/**
 * 
 */
UCLASS()
class MAGE_API UMageAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void AbilityActorInfoIsSet();
	void AddCharacterAbilites(TArray<TSubclassOf<UGameplayAbility>>& AbilitiesClass);
	void AddCharacterPassiveAbilites(TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilitiesClass);

	FEffectAssetTagsSignature EffectAssetTagsDelegate;
	
	/** Ability */
	void AbilityInputHeld(const FGameplayTag& InputTag);
	void AbilityInputReleased(const FGameplayTag& InputTag);

	/** Abilities Data */
	bool bStartupAbilitiesGiven = false; // 用于记录当前技能是否初始化完成
	FAbilitiesGivenSignature AbilitiesGivenDelegate; // 技能赋予完毕回调委托
	void ForeachAbilitiesExecute(const FForeachAbilitiesSignature& ForeachAbilities); // 遍历执行技能回调委托
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	/** Attribute */
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);
protected:
	virtual void OnRep_ActivateAbilities() override;
	UFUNCTION(Client, Reliable)
	void ClientOnEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
};
