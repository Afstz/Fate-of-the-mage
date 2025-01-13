// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "DamageGameplayAbility.h"
#include "ProjectileSpellAbility.generated.h"

class AMageProjectile;
/**
 * 
 */
UCLASS()
class MAGE_API UProjectileSpellAbility : public UDamageGameplayAbility
{
	GENERATED_BODY()
public:

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AMageProjectile> ProjectileClass;
private:
	UFUNCTION(BlueprintCallable)
	virtual void SpawnProjectile(const FVector& ProjectileLocation, const bool bOverridePitch = false, float PitchOverride = 0.f);
	
};
