// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/SaveInterface.h"
#include "EnemySpawnTrigger.generated.h"

class AEnemySpawnPoint;
class UBoxComponent;

UCLASS()
class MAGE_API AEnemySpawnTrigger : public AActor, public ISaveInterface
{
	GENERATED_BODY()

public:
	AEnemySpawnTrigger();

	/** Save Interface */
	virtual void LoadActor_Implementation() override;

	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bTriggered = false;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	TArray<AEnemySpawnPoint*> EnemySpawnPoints;
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxTrigger;
};