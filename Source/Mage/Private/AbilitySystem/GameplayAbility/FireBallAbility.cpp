// Copyright AFstz.


#include "AbilitySystem/GameplayAbility/FireBallAbility.h"
#include "AbilitySystem/MageAbilitySystemLibrary.h"
#include "Actor/MageProjectile.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interface/CombatInterface.h"

FString UFireBallAbility::GetDescription(int32 AbilityLevel)
{
	float CausedDamage = GetAbilityDamage(AbilityLevel);
	float ManaCost = GetManaCost(AbilityLevel);
	float CooldownTime = GetCooldownTime(AbilityLevel);
	MaxNumProjectiles = FMath::Min(MaxNumProjectiles, AbilityLevel);
	
	return FString::Printf(TEXT(
		// Title
		"<Title>火球术</>\n\n"
		"<Default>技能等级:</><Level>%d</>\n"
		"<Default>伤害:</><Damage>%.0f</>\n"
		"<Default>耗蓝量:</><ManaCost>%.1f</>\n"
		"<Default>冷却时间:</><CooldownTime>%.1f</>\n\n"
		// Description
		"<Small>发射 %d 个火球，击中敌人发生爆炸。</>")
		
		, AbilityLevel, CausedDamage, ManaCost, CooldownTime, MaxNumProjectiles);

}

FString UFireBallAbility::GetNextLevelDescription(int32 AbilityLevel)
{
	float CausedDamage = GetAbilityDamage(AbilityLevel);
	float ManaCost = GetManaCost(AbilityLevel);
	float CooldownTime = GetCooldownTime(AbilityLevel);
	MaxNumProjectiles = FMath::Min(MaxNumProjectiles, AbilityLevel);
	
	return FString::Printf(TEXT(
		// Title
		"<Title>下一个等级</>\n\n"
		"<Default>技能等级:</><Level>%d</>\n"
		"<Default>伤害:</><Damage>%.0f</>\n"
		"<Default>耗蓝量:</><ManaCost>%.1f</>\n"
		"<Default>冷却时间:</><CooldownTime>%.1f</>\n\n"
		// Description
		"<Small>发射 %d 个火球，击中敌人发生爆炸。造成更高的伤害。</>")
		
		, AbilityLevel, CausedDamage, ManaCost, CooldownTime, MaxNumProjectiles);
}

void UFireBallAbility::SpawnProjectiles(const FVector& TargetLocation, const bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	APawn* Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());
	
	const FVector SpawnLocation = ICombatInterface::Execute_GetLocationByWeaponSocket(Instigator);
	FRotator SpawnRotation = (TargetLocation - SpawnLocation).Rotation();
	if (bOverridePitch)
	{
		SpawnRotation.Pitch = PitchOverride;
	}
	const FVector Forward = SpawnRotation.Vector(); // 目标方向
	
	float MaxNumProjectile = FMath::Min(MaxNumProjectiles, GetAbilityLevel());

	// 等间距旋转
	TArray<FRotator> Rotators = UMageAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, SpawnSpread, MaxNumProjectile); 
	for (FRotator& Rot : Rotators)
	{
		// 延迟生成炮弹
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SpawnLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());
		AMageProjectile* Projectile = GetWorld()->SpawnActorDeferred<AMageProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Instigator,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);
		
		// 导航
		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent(); // 寻航组件
		}
		else
		{
			// 不是角色实体
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(TargetLocation);
			Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}
		// 寻航速度
		Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::RandRange(MinHomingAcceleration, MaxHomingAcceleration); 
		Projectile->ProjectileMovement->bIsHomingProjectile = bHomingTarget; // 炮弹是否导航到目标
		
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}