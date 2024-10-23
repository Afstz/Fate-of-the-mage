// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MageAbilitySystemLibrary.generated.h"

class UAbilitySystemComponent;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
/**
 *  用于暴露给蓝图的便捷静态函数库
 */
UCLASS()
class MAGE_API UMageAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "MageAbilitySystemLibrary | WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category = "MageAbilitySystemLibrary | WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | CharacterClassDefaults")
	static void InitCharacterDefaultAttributes(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass, int32 Level);
};
