// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MageAbilitySystemComponent.generated.h"

class UMageAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagsSignature, const FGameplayTagContainer& /* Asset Tags */);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGivenSignature); // 技能初始化委托
DECLARE_DELEGATE_OneParam(FForeachAbilitiesSignature, const FGameplayAbilitySpec&); // 遍历技能单播委托

DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /* Ability Tag*/, const FGameplayTag& /* Status Tag*/, int32 /* New Level*/);
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquippedSignature, const FGameplayTag& /* Ability Tag*/, const FGameplayTag& /* Status Tag*/, const FGameplayTag& /* Input Tag*/, const FGameplayTag& /* Prev Input Tag*/);
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
	void AbilityInputPressed(const FGameplayTag& InputTag);
	void AbilityInputHeld(const FGameplayTag& InputTag);
	void AbilityInputReleased(const FGameplayTag& InputTag);

	/** Abilities Data */
	bool bStartupAbilitiesGiven = false; // 用于记录当前技能是否初始化完成
	FAbilitiesGivenSignature AbilitiesGivenDelegate; // 技能赋予完毕回调委托
	void ForeachAbilitiesExecute(const FForeachAbilitiesSignature& ForeachAbilities); // 遍历执行技能回调委托
	
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetStatusTagFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag);

	/** Skill */
	FAbilityStatusChanged AbilityStatusChanged;
	FAbilityEquippedSignature AbilityEquippedDelegate;
	void UpdateAbilityStatuses(const int32 InPlayerLevel);
	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel);
	UFUNCTION(Server, Reliable)
	void ServerSpendSkillPoint(const FGameplayTag& AbilityTag);
	UFUNCTION(Server, Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag);
	UFUNCTION(Client, Reliable)
	void ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& InputTag, const FGameplayTag& PrevInputTag);
	void ClearSlot(FGameplayAbilitySpec* AbilitySpec);
	void ClearAbilitiesOfSlot(const FGameplayTag& InputTag);
	bool AbilityHasInputTag(const FGameplayAbilitySpec* AbilitySpec, const FGameplayTag& InputTag);
	
	
	/** Attribute */
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);
protected:
	virtual void OnRep_ActivateAbilities() override;
	UFUNCTION(Client, Reliable)
	void ClientOnEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
};

