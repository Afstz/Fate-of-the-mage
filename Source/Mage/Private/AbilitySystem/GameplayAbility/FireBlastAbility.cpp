// Copyright AFstz.


#include "AbilitySystem/GameplayAbility/FireBlastAbility.h"

#include "AbilitySystem/MageAbilitySystemLibrary.h"
#include "Actor/FireBlast.h"

FString UFireBlastAbility::GetDescription(int32 AbilityLevel)
{
	float CausedDamage = GetAbilityDamage(AbilityLevel);
	float ManaCost = GetManaCost(AbilityLevel);
	float CooldownTime = GetCooldownTime(AbilityLevel);
	
	return FString::Printf(TEXT(
		// Title
		"<Title>火球冲击</>\n\n"
		"<Default>技能等级:</><Level>%d</>\n"
		"<Default>伤害:</><Damage>%.0f</>\n"
		"<Default>耗蓝量:</><ManaCost>%.1f</>\n"
		"<Default>冷却时间:</><CooldownTime>%.1f</>\n\n"
		// Description
		"<Small>发射 %d 个火球，这些火球会向四周扩散，最后会折反回来发生一次爆炸伤害，有几率造成点燃和击退。</>")
		
		, AbilityLevel, CausedDamage, ManaCost, CooldownTime, NumFireBalls);
}

FString UFireBlastAbility::GetNextLevelDescription(int32 AbilityLevel)
{
	float CausedDamage = GetAbilityDamage(AbilityLevel);
	float ManaCost = GetManaCost(AbilityLevel);
	float CooldownTime = GetCooldownTime(AbilityLevel);
	
	return FString::Printf(TEXT(
		// Title
		"<Title>下一个等级</>\n\n"
		"<Default>技能等级:</><Level>%d</>\n"
		"<Default>伤害:</><Damage>%.0f</>\n"
		"<Default>耗蓝量:</><ManaCost>%.1f</>\n"
		"<Default>冷却时间:</><CooldownTime>%.1f</>\n\n"
		// Description
		"<Small>发射 %d 个火球，这些火球会向四周扩散，最后会折反回来发生一次爆炸伤害，升级造成更高的伤害。</>")
		
		, AbilityLevel, CausedDamage, ManaCost, CooldownTime, NumFireBalls);
}

TArray<AFireBlast*> UFireBlastAbility::SpawnFireBlast()
{
	TArray<AFireBlast*> FireBalls;
	bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return FireBalls;
	
	const FVector SpawnLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	TArray<FRotator> Rotators = UMageAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, 360.f, NumFireBalls);

	for (int i = 0; i < NumFireBalls - 1; i++) // [NumFireBalls - 1]是因为360°最后一个火球会和开始生成的火球重叠
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SpawnLocation);
		SpawnTransform.SetRotation(Rotators[i].Quaternion());
		
		AFireBlast* FireBall = GetWorld()->SpawnActorDeferred<AFireBlast>(
			FireBallClass,
			SpawnTransform,
			GetAvatarActorFromActorInfo(),
			GetActorInfo().PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		FireBalls.Add(FireBall);
		
		FireBall->FinishSpawning(SpawnTransform);
	}
	
	return FireBalls;
}