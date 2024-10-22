// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbility/MageGameplayAbility.h"
#include "ProjectileSpellAbility.generated.h"

class AMageProjectile;
/**
 * 
 */
UCLASS()
class MAGE_API UProjectileSpellAbility : public UMageGameplayAbility
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
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
};
