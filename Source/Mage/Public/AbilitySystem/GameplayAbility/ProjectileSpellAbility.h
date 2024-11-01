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
private:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AMageProjectile> ProjectileClass;
	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(const FVector& ProjectileLocation);
	
};
