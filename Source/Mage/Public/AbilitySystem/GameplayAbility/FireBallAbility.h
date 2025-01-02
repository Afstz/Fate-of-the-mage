// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbility/ProjectileSpellAbility.h"
#include "FireBallAbility.generated.h"

/**
 * 
 */
UCLASS()
class MAGE_API UFireBallAbility : public UProjectileSpellAbility
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 AbilityLevel) override;
	virtual FString GetNextLevelDescription(int32 AbilityLevel) override;
};
