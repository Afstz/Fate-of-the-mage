// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "Character/MageCharacterBase.h"
#include "Interface/PlayerInterface.h"
#include "MageCharacte.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class MAGE_API AMageCharacte : public AMageCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
public:
	AMageCharacte();

	virtual void PossessedBy(AController* NewController) override; // 只会在服务器执行
	virtual void OnRep_PlayerState() override;
	
	/** Combat Interface */
	virtual int32 GetCharacterLevel_Implementation() const override;

	/** Player Interface */
	virtual void LevelUp_Implementation() override;
	virtual int32 FindLevelForXP_Implementation(const int32 InXP) override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 GetAttributePointReward_Implementation(const int32 InLevel) const override;
	virtual int32 GetSkillPointReward_Implementation(const int32 InLevel) const override;
	virtual int32 GetAttributePoint_Implementation() const override;
	virtual int32 GetSkillPoint_Implementation() const override;
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void AddToLevel_Implementation(int32 InLevel) override;
	virtual void AddToAttributePoint_Implementation(int32 InAttributePoint) override;
	virtual void AddToSkillPoint_Implementation(int32 InSkillPoint) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;
	
	virtual void InitAbilityActorInfo() override;
	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpEffect();
};
