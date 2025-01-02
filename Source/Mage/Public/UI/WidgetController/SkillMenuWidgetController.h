// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "MageGameplayTags.h"
#include "UI/WidgetController/MageWidgetController.h"
#include "SkillMenuWidgetController.generated.h"

struct FMageGameplayTags;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSphereSelectedSignature, bool, bShouldSkillPointButtonEnable, bool, bShouldEquipButtonEnable, FString, Description, FString, NextLevelDescription);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSignature, const FGameplayTag&, AbilityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStopWaitForEquipSignature, const FGameplayTag&, AbilityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityReassignedSignature, const FGameplayTag&, AbilityTag);

struct FSelectedAbility
{
	FGameplayTag AbilityTag = FGameplayTag();
	FGameplayTag StatusTag = FGameplayTag();
};
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MAGE_API USkillMenuWidgetController : public UMageWidgetController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature OnSkillPointChangedDelegate;
	UPROPERTY(BlueprintAssignable)
	FSphereSelectedSignature SphereSelectedDelegate;
	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSignature WaitForEquipDelegate;
	UPROPERTY(BlueprintAssignable)
	FStopWaitForEquipSignature StopWaitForEquipDelegate;
	UPROPERTY(BlueprintAssignable)
	FAbilityReassignedSignature AbilityReassignedDelegate;
	
	virtual void BroadcastInitialValue() override;
	virtual void BindCallbacksToDependencies() override;
protected:
	virtual void ClearAllDelegate() override;
	
	UFUNCTION(BlueprintCallable)
	void OnSphereButtonSelected(const FGameplayTag& AbilityTag);
	UFUNCTION(BlueprintCallable)
	void SpendSkillPointPressed();
	UFUNCTION(BlueprintCallable)
	void SkillSphereDeselect();
	
	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();
	UFUNCTION(BlueprintCallable)
	void EquipRowSpherePressed(const FGameplayTag& AbilityType, const FGameplayTag& InputTag);

private:
	void ShouldButtonEnables(int32 SkillPoint, const FGameplayTag& StatusTag, bool& bShouldSkillPointEnable, bool& bShouldEquipButtonEnable);
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& InputTag, const FGameplayTag& PrevInputTag);

	// 记录当前选中技能的信息
	FSelectedAbility SelectedAbility { FMageGameplayTags::Get().Abilities_None, FMageGameplayTags::Get().Abilities_Status_Locked};
	int32 CurrentSkillPoint = 0;

	bool bWaitingForEquip = false;
	FGameplayTag SelectedSlot;
};
