// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassData.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"


class UNiagaraSystem;
// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MAGE_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat Interface")
	int32 GetCharacterLevel() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat Interface")
	FVector GetLocationByWeaponSocket() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat Interface")
	UAnimMontage* GetHitReactMontage() const;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Combat Interface")
	void UpdateFacingTarget(const FVector& TargetLocation); // 更新要面向的目标的位置
	
	virtual void Die() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat Interface")
	bool IsDead() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat Interface")
	AActor* GetAvatarActor();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat Interface")
	UNiagaraSystem* GetBloodEffect();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat Interface")
	int32 GetMinionCount() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat Interface")
	void AddMinionCount(const int32 InMinionCount);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat Interface")
	ECharacterClass GetCharacterClass() const;
};
