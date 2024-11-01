// Copyright AFstz.


#include "AbilitySystem/GameplayAbility/ProjectileSpellAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/MageProjectile.h"
#include "Interface/CombatInterface.h"
#include "MageGameplayTags.h"

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
		// 设置发射物位置和方向
		FTransform SpawnTransform;
		FVector SpawnLocation = CombatInterface->GetLocationByWeaponSocket();
		FRotator SpawnRotation = (ProjectileLocation - SpawnLocation).Rotation();
		SpawnTransform.SetLocation(SpawnLocation);
		SpawnTransform.SetRotation(SpawnRotation.Quaternion());
		
		AMageProjectile* Projectile = GetWorld()->SpawnActorDeferred<AMageProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Instigator,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		// 设置游戏效果和伤害
		UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

		for (auto& Pair : DamageTypes)
		{
			const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage); 
		}
		
		Projectile->EffectSpecHandle = SpecHandle;
		Projectile->FinishSpawning(SpawnTransform);
	}
}