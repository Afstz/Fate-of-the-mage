// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Interface/CombatInterface.h"
#include "GameFramework/Character.h"
#include "MageCharacterBase.generated.h"

class UMotionWarpingComponent;
class UGameplayAbility;
class UAttributeSet;
class UGameplayEffect;

UCLASS(Abstract)
class MAGE_API AMageCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AMageCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	/** Combat Interface */
	virtual FVector GetLocationByWeaponSocket() const override;
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultBaseEffects;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryEffects;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryEffects;
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect>& InitAttributeEffects, float Level);
	virtual void InitDefaultAttributes();

	void AddCharacterAbilites();
private:
	UPROPERTY(EditDefaultsOnly, Category = "Abilites")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilites;
};
