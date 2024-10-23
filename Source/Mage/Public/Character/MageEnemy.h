// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassData.h"
#include "Character/MageCharacterBase.h"
#include "Interface/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "MageEnemy.generated.h"

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class MAGE_API AMageEnemy : public AMageCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
public:
	AMageEnemy();

	/** Enemy Hightlight Interface. */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	/** Combat Interface. */
	FORCEINLINE virtual int32 GetCharacterLevel() const override { return Level; }

	/** Widget */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
	TObjectPtr<UWidgetComponent> HealthBar;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature HealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature MaxHealthChanged;
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Attributes")
	int32 Level = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Attributes")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	virtual void InitDefaultAttributes() override;
private:
	/** Widget */
	void InitHealthBar();
};
