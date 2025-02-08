// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MAGE_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(const int32 InXP);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP() const;
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointReward(const int32 InLevel) const;
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSkillPointReward(const int32 InLevel) const;
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoint() const;
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSkillPoint() const;
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 InXP);
	UFUNCTION(BlueprintNativeEvent)
	void AddToLevel(int32 InLevel);
	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoint(int32 InAttributePoint);
	UFUNCTION(BlueprintNativeEvent)
	void AddToSkillPoint(int32 InSkillPoint);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideMagicCircle();
};