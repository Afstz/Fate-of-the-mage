// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassData.h"
#include "Engine/TargetPoint.h"
#include "EnemySpawnPoint.generated.h"

class AMageEnemy;

UCLASS()
class MAGE_API AEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	AEnemySpawnPoint();

	void SpawnEnemy();

protected:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AMageEnemy> EnemyClass;
	UPROPERTY(EditAnywhere)
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	UPROPERTY(EditAnywhere)
	int32 Level = 1;
};