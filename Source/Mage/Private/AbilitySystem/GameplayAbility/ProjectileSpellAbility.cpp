// Copyright AFstz.


#include "AbilitySystem/GameplayAbility/ProjectileSpellAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
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
	// 设置发射物位置和方向
	FTransform SpawnTransform;
	FVector SpawnLocation = ICombatInterface::Execute_GetLocationByWeaponSocket(Instigator); // NativeEvent必须用静态函数
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
		const float DamageMagnitude = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, DamageMagnitude); 
	}
		
	Projectile->EffectSpecHandle = SpecHandle;
	Projectile->FinishSpawning(SpawnTransform);
}