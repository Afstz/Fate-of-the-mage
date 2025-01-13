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

void UProjectileSpellAbility::SpawnProjectile(const FVector& TargetLocation, const bool bOverridePitch, float PitchOverride)
{
	bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	APawn* Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());
	// 设置发射物位置和方向
	FTransform SpawnTransform;
	FVector SpawnLocation = ICombatInterface::Execute_GetLocationByWeaponSocket(Instigator); // NativeEvent必须用静态函数
	FRotator SpawnRotation = (TargetLocation - SpawnLocation).Rotation();
	if (bOverridePitch)
	{
		SpawnRotation.Pitch = PitchOverride;
	}
	
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(SpawnRotation.Quaternion());
		
	AMageProjectile* Projectile = GetWorld()->SpawnActorDeferred<AMageProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Instigator,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults(); // 创建伤害结构体用于计算
		
	Projectile->FinishSpawning(SpawnTransform);
}