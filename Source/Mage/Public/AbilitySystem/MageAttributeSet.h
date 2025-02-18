 // Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MageAttributeSet.generated.h"

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()
	
	// Source
	FGameplayEffectContextHandle SourceEffectContextHandle;
	FGameplayEffectSpec SourceEffectSpec;
	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;
	UPROPERTY() 
	AActor* SourceAvatarActor = nullptr;
	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;
	UPROPERTY()
	AController* SourceController = nullptr;

	// Target
	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;
	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
	UPROPERTY()
	AController* TargetController = nullptr;
};

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/* 静态函数指针
using AttributeFuncPtr = TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr;
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;
 */

/**
 * 
 */
UCLASS()
class MAGE_API UMageAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UMageAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 当Attribute的CurrentValue被改变之前调用
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	// 仅在GameplayEffect使Attribute的 BaseValue 改变后触发。
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	/** Attribute Menu */
	TMap<FGameplayTag, FGameplayAttribute> TagsToAttributes; // 属性标签及对应的GetAttribute方法
	
	/*
	 * Character Base Attributes
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Player Attributes | Base")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMageAttributeSet, Health); // 为每一个属性添加对应的辅助函数
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Player Attributes | Base")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UMageAttributeSet, Mana);

	/*
	 * Character Primary Attributes
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Player Attributes | Primary")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UMageAttributeSet, Strength);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Player Attributes | Primary")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UMageAttributeSet, Intelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Player Attributes | Primary")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UMageAttributeSet, Resilience);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Player Attributes | Primary")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UMageAttributeSet, Vigor);

	/*
	 * Character Secondary Attributes
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Player Attributes | Secondary")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UMageAttributeSet, Armor);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Player Attributes | Secondary")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UMageAttributeSet, ArmorPenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Player Attributes | Secondary")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UMageAttributeSet, BlockChance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Player Attributes | Secondary")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UMageAttributeSet, CriticalHitChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Player Attributes | Secondary")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UMageAttributeSet, CriticalHitDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "Player Attributes | Secondary")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UMageAttributeSet, CriticalHitResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Player Attributes | Secondary")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UMageAttributeSet, HealthRegeneration);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Player Attributes | Secondary")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UMageAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Player Attributes | Secondary")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UMageAttributeSet, ManaRegeneration);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Player Attributes | Secondary")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UMageAttributeSet, MaxMana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistence, Category = "Player Attributes | Secondary")
	FGameplayAttributeData PhysicalResistence;
	ATTRIBUTE_ACCESSORS(UMageAttributeSet, PhysicalResistence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicalResistence, Category = "Player Attributes | Secondary")
	FGameplayAttributeData MagicalResistence;
	ATTRIBUTE_ACCESSORS(UMageAttributeSet, MagicalResistence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireResistence, Category = "Player Attributes | Secondary")
	FGameplayAttributeData FireResistence;
	ATTRIBUTE_ACCESSORS(UMageAttributeSet, FireResistence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LightningResistence, Category = "Player Attributes | Secondary")
	FGameplayAttributeData LightningResistence;
	ATTRIBUTE_ACCESSORS(UMageAttributeSet, LightningResistence);

	/*
	 * Meta Attribute
	 */

	UPROPERTY(BlueprintReadOnly, Category = "Player Attributes | Meta")
	FGameplayAttributeData ReceivedDamage; // 处理传过来的伤害
	ATTRIBUTE_ACCESSORS(UMageAttributeSet, ReceivedDamage);
	UPROPERTY(BlueprintReadOnly, Category = "Player Attributes | Meta")
	FGameplayAttributeData ReceivedXP;
	ATTRIBUTE_ACCESSORS(UMageAttributeSet, ReceivedXP);
	
	/*
	 * OnRep Functions
	 */
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;
	
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;
	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;
	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;
	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	UFUNCTION()
	void OnRep_PhysicalResistence(const FGameplayAttributeData& OldPhysicalResistence) const;
	UFUNCTION()
	void OnRep_MagicalResistence(const FGameplayAttributeData& OldMagicalResistence) const;
	UFUNCTION()
	void OnRep_FireResistence(const FGameplayAttributeData& OldFireResistence) const;
	UFUNCTION()
	void OnRep_LightningResistence(const FGameplayAttributeData& OldLightningResistence) const;
	
	
private:
	/*
	 * Other Function
	 */
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProperties);
	void ShowCharacterDamageText(const FEffectProperties& EffectProperties, const float DamageValue,
		const bool bIsCriticalHit, const bool bIsBlockHit) const; // 显示对对方造成的伤害
	void SendXPRewardEvent(const FEffectProperties& EffectProperties);
	void HandleReceivedXP(const FEffectProperties& EffectProps);
	void HandleReceivedDamage(const FEffectProperties& EffectProps);
	void HandleDebuff(const FEffectProperties& EffectProps);

	/** Level */
	
	// 升级时延迟填充血量和蓝量
	bool bFillInHealth = false; 
	bool bFillInMana = false;
};
