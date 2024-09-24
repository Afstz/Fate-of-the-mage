// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "Character/MageCharacterBase.h"
#include "MageCharacte.generated.h"

/**
 * 
 */
UCLASS()
class MAGE_API AMageCharacte : public AMageCharacterBase
{
	GENERATED_BODY()
public:
	AMageCharacte();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	/** Combat Interface. */
	virtual int32 GetCharacterLevel() const override;
	
private:
	virtual void InitAbilityActorInfo() override;
	
};
