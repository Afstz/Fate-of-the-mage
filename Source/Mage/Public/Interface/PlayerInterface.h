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
	int32 GetAttributePoint(const int32 InLevel) const;
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSkillPoint(const int32 InLevel) const;
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 InXP);
	UFUNCTION(BlueprintNativeEvent)
	void AddToLevel(int32 InLevel);
	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoint(int32 InAttributePoint);
	UFUNCTION(BlueprintNativeEvent)
	void AddToSkillPoint(int32 InSkillPoint);

	
};