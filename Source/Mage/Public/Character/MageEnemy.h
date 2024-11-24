// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassData.h"
#include "Character/MageCharacterBase.h"
#include "Interface/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "MageEnemy.generated.h"

class AMageAIController;
class UBehaviorTree;
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
	virtual void PossessedBy(AController* NewController) override;

	/** Enemy Interface. */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	virtual AActor* GetFacingTarget_Implementation() const override;
	virtual void SetFacingTarget_Implementation(AActor* InFacingTarget) override;
	virtual FVector GetSocketLocationByStruct_Implementation(const FTaggedMontage& TaggedMontage) const override;
	virtual TArray<FTaggedMontage> GetTaggedMontage_Implementation() const override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;

	/** Combat Interface. */
	FORCEINLINE virtual int32 GetCharacterLevel() const override { return Level; }
	virtual void Die() override;

	/** Widget */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UWidgetComponent> HealthBar;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature HealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature MaxHealthChanged;

	/** HitReact */
	void HitReactCallBack(const FGameplayTag HitReactTag, int32 NewCount);
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReact = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float LifeSpan = 5.f;
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Attributes")
	int32 Level = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Attributes")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	virtual void InitDefaultAttributes() override;
	/** AI */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	UPROPERTY()
	TObjectPtr<AMageAIController> MageAIController;

	/** Motion Warping*/
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> FacingTarget;
	
	/** Montage */
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TArray<FTaggedMontage> AttackMontages; // 存储着蒙太奇动画标签结构体
private:
	/** Widget */
	void InitHealthBar();
	UFUNCTION(NetMulticast, Reliable)
	void MultiHiddenWidget();
};
