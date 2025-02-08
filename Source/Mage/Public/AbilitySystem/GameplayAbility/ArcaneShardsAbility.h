// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbility/DamageGameplayAbility.h"
#include "ArcaneShardsAbility.generated.h"

/**
 * 
 */
UCLASS()
class MAGE_API UArcaneShardsAbility : public UDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual FString GetDescription(int32 AbilityLevel) override;
	virtual FString GetNextLevelDescription(int32 AbilityLevel) override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxNumShards = 9.f;
	UFUNCTION(BlueprintPure, BlueprintCallable)
	int32 GetCurrentShardsNum(int32 InAbilityLevel);
};
