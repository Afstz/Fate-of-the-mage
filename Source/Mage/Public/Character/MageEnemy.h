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

	/** Combat Interface. */
	FORCEINLINE virtual int32 GetCharacterLevel() const override { return Level; }

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Attribute")
	int32 Level = 1;
};
