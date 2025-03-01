// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/MageAbilitySystemComponent.h"
#include "Character/MageCharacterBase.h"
#include "Interface/PlayerInterface.h"
#include "Player/MagePlayerState.h"
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
	virtual void ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial = nullptr) override;
	virtual void HideMagicCircle_Implementation() override;
	virtual void SaveGameProgress_Implementation(const FName& CheckPointTag) override;

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	/** Combat */
	virtual void OnRep_Stunned(bool OldStunned) override;
	virtual void OnStunTagChanged(const FGameplayTag StunTag, int32 NewCount) override;

	/** Deid Status */
	virtual void Die(const FVector& InDeathImpulse) override;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float DeathTime = 3.f;
	FTimerHandle DeathTimer;

	/** Save Game */
	void LoadGameProgress();

	/** Respawn */
	void PlayerRespawnInit(AMagePlayerState* MagePlayerState, UMageAbilitySystemComponent* MageASC);
	void ClientPlayerRespawnInit(AMagePlayerState* MagePlayerState, UMageAbilitySystemComponent* MageASC);
	void ClientPlayerFirstInit(AMagePlayerState* MagePlayerState);
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;
	
	virtual void InitAbilityActorInfo() override;
	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpEffect();
};
