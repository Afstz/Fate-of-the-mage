// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Interface/CombatInterface.h"
#include "GameFramework/Character.h"
#include "MageCharacterBase.generated.h"

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
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

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
	void InitDefaultAttributes();
	
};
