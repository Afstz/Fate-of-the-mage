// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "MagePlayerState.generated.h"

class ULevelUpData;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /* Statistic Value*/);

/**
 * 
 */
UCLASS()
class MAGE_API AMagePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AMagePlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Delegate */
	FOnPlayerStatChanged LevelChangedDelegate;
	FOnPlayerStatChanged XPChangedDelegate;
	FOnPlayerStatChanged AttributePointChangedDelegate;
	FOnPlayerStatChanged SkillPointChangedDelegate;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpData> LevelUpData; // 等级升级数据

	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetPlayerXP() const { return XP; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSkillPoints() const { return SkillPoints; }
	void SetLevel(const int32 InLevel);
	void AddToLevel(const int32 InLevel);
	void SetXP(const int32 InXP);
	void AddToXP(const int32 InXP);
	void AddToAttributePoints(const int32 InAttributePoints);
	void AddToSkillPoints(const int32 InSkillPoints);
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_XP)
	int32 XP = 0;
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_AttributePoints)
	int32 AttributePoints = 4;
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_SkillPoints)
	int32 SkillPoints = 0;
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	UFUNCTION()
	void Onrep_XP(int32 OldXP);
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);
	UFUNCTION()
	void OnRep_SkillPoints(int32 OldSkillPoints);
};
