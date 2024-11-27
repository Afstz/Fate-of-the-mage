// Copyright AFstz.


#include "AbilitySystem/GameplayAbility/SummonAbility.h"

TArray<FVector> USummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector ActorLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / MinionNum; // 每个仆从之间的间隔

	TArray<FVector> SpawnLocations;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector); // 从左到右生成
	for (int i = 0; i < MinionNum; i++)
	{
		// 生成召唤单位方向向量
		const FVector SpawnDirection = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		// 在范围内随机选取一个直线上的点
		FVector RandomSpawnLocation = ActorLocation + SpawnDirection * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, RandomSpawnLocation + FVector(0.f, 0.f, 300.f), RandomSpawnLocation - FVector(0.f, 0.f, 300.f), ECC_Visibility);
		if (HitResult.bBlockingHit) // 从上至下判断是否有地方生成
		{
			SpawnLocations.Add(HitResult.ImpactPoint);
		}
	}
	
	return SpawnLocations;
}

TSubclassOf<APawn> USummonAbility::GetRandomMinionClass()
{
	const int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[Selection];
}