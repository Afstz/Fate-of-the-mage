// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "Character/MageCharacterBase.h"
#include "Interface/EnemyInterface.h"
#include "MageEnemy.generated.h"

/**
 * 
 */
UCLASS()
class MAGE_API AMageEnemy : public AMageCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
public:
	AMageEnemy();

	/** Enemy Hightlight Interface. */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
};
