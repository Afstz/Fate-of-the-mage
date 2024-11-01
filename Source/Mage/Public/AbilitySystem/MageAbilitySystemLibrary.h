// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
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
	
	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | CharacterClassDefaults")
	static void GiveCharacterAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | CharacterClassDefaults")
	static UCharacterClassData* GetCharacterClassData(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayEffects")
	static bool GetIsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayEffects")
	static bool GetIsBlockHit(const FGameplayEffectContextHandle& EffectContextHandle);

	// UPARAM(ref) 可以让蓝图输出引脚改成输入，不加默认引用参数为输出
	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayEffects")
	static void SetIsBlockHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsBlockHit);
};
