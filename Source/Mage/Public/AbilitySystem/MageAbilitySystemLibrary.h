// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Data/CharacterClassData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MageAbilitySystemLibrary.generated.h"

struct FDamageEffectParams;
class UAbilityData;
class USkillMenuWidgetController;
class AMageHUD;
struct FWidgetControllerParams;
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
	UFUNCTION(BlueprintPure, Category = "MageAbilitySystemLibrary | WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AMageHUD*& OutMageHUD);
	
	UFUNCTION(BlueprintPure, Category = "MageAbilitySystemLibrary | WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "MageAbilitySystemLibrary | WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "MageAbilitySystemLibrary | WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static USkillMenuWidgetController* GetSkillMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | CharacterClassDefaults")
	static void InitCharacterDefaultAttributes(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass, int32 Level);
	
	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | CharacterClassDefaults")
	static void GiveCharacterAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | CharacterClassDefaults")
	static UCharacterClassData* GetCharacterClassData(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | AbilityData")
	static UAbilityData* GetAbilityData(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayEffects")
	static bool GetCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayEffects")
	static bool GetBlockHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayEffects")
	static bool GetSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayEffects")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayEffects")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayEffects")
	static float GetDebuffFrequence(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayEffects")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayEffects")
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayEffects")
	static FVector GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle);
	
	// UPARAM(ref) 可以让蓝图输出引脚改成输入，不加默认引用参数为输出
	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayEffects")
	static void SetCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayEffects")
	static void SetBlockHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsBlockHit);

	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayEffects")
	static void SetSuccessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInSuccessfulDebuff);

	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayEffects")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage);

	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayEffects")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration);

	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayEffects")
	static void SetDebuffFrequence(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequence);

	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayEffects")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, FGameplayTag InDamageType);

	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayEffects")
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, FVector InDeathImpulse);

	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayEffects")
	static void SetKnockbackForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, FVector InKnockbackForce);
	
	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayEffects")
	static void ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);
	
	UFUNCTION(BlueprintCallable, Category = "MageAbilitySystemLibrary | GameplayMechanics")
	static void GetAlivePlayerInSphereRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& InActorsToIgnore, float SphereRadius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category = "MageAbilitySystemLibrary | GameplayMechanics")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);

	UFUNCTION(BlueprintPure, Category = "MageAbilitySystemLibrary | GameplayMechanics")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& RotationAxis, float Spread, int32 NumRotators);
	
	UFUNCTION(BlueprintPure, Category = "MageAbilitySystemLibrary | GameplayMechanics")
	static TArray<FVector> EvenlyRotatedVectors(const FVector& Forward, const FVector& RotationAxis, float Spread, int32 NumVectors);
	
	UFUNCTION(BlueprintPure, Category = "MageAbilitySystemLibrary | XP")
	static int32 GetXPRewardForClassAndLevel(UObject* WorldContextObject, ECharacterClass CharacterClass, int32 Level);
};
