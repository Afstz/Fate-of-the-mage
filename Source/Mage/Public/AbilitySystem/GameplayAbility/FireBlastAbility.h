// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "DamageGameplayAbility.h"
#include "FireBlastAbility.generated.h"

class AFireBlast;
/**
 * 
 */
UCLASS()
class MAGE_API UFireBlastAbility : public UDamageGameplayAbility
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 AbilityLevel) override;
	virtual FString GetNextLevelDescription(int32 AbilityLevel) override;

	UFUNCTION(BlueprintCallable)
	TArray<AFireBlast*> SpawnFireBlast();
	
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 NumFireBalls = 12;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFireBlast> FireBallClass;
};
