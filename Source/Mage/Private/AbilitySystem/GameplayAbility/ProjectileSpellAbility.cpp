// Copyright AFstz.


#include "AbilitySystem/GameplayAbility/ProjectileSpellAbility.h"

#include "AbilitySystemComponent.h"
#include "Actor/MageProjectile.h"
#include "Interface/CombatInterface.h"

void UProjectileSpellAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo,
                                              const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
}

void UProjectileSpellAbility::SpawnProjectile(const FVector& ProjectileLocation)
{
	bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	APawn* Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Instigator))
	{
		FTransform SpawnTransform;
		FVector SpawnLocation = CombatInterface->GetLocationByWeaponSocket();
		FRotator SpawnRotation = (ProjectileLocation - SpawnLocation).Rotation();
		SpawnRotation.Pitch = 0;
		SpawnTransform.SetLocation(SpawnLocation);
		SpawnTransform.SetRotation(SpawnRotation.Quaternion());
		
		AMageProjectile* Projectile = GetWorld()->SpawnActorDeferred<AMageProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Instigator,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
		Projectile->EffectSpecHandle = ASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), ASC->MakeEffectContext());
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}