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
	
	AMageEnemy* SpawnedEnemy = GetWorld()->SpawnActorDeferred<AMageEnemy>(
		EnemyClass,
		GetActorTransform(),
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	// 指定敌人等级和类型
	SpawnedEnemy->SetLevel(Level);
	SpawnedEnemy->SetCharacterClass(CharacterClass);
	SpawnedEnemy->FinishSpawning(GetActorTransform());
	SpawnedEnemy->SpawnDefaultController();
}



