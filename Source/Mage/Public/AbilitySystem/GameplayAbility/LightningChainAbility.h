// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbility/BeamSpellAbility.h"
#include "LightningChainAbility.generated.h"

/**
 * 
 */
UCLASS()
class MAGE_API ULightningChainAbility : public UBeamSpellAbility
{
	GENERATED_BODY()
	
public:
	virtual FString GetDescription(int32 AbilityLevel) override;
	virtual FString GetNextLevelDescription(int32 AbilityLevel) override;
protected:
};
