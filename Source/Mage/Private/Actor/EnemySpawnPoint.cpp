// Copyright AFstz.


#include "Actor/EnemySpawnPoint.h"

#include "Character/MageEnemy.h"

AEnemySpawnPoint::AEnemySpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemySpawnPoint::SpawnEnemy()
{
	if (!HasAuthority()) return;
	FTransform SpawnTransform;
	FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 10.f);
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(GetActorRotation().Quaternion());
	AMageEnemy* SpawnedEnemy = GetWorld()->SpawnActorDeferred<AMageEnemy>(
		EnemyClass,
		SpawnTransform,
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	// 指定敌人等级和类型
	SpawnedEnemy->SetLevel(Level);
	SpawnedEnemy->SetCharacterClass(CharacterClass);
	SpawnedEnemy->FinishSpawning(SpawnTransform);
	SpawnedEnemy->SpawnDefaultController();
}



