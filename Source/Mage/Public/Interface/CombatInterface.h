// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
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
	virtual int32 GetCharacterLevel() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat")
	FVector GetLocationByWeaponSocket() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat")
	UAnimMontage* GetHitReactMontage() const;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateFacingTarget(const FVector& TargetLocation);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UNiagaraSystem* GetBloodEffect();
	
	virtual void Die() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsDead() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetAvatarActor();
};
