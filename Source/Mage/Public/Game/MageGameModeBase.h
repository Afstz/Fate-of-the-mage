// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MageGameModeBase.generated.h"

class UAbilityData;
class UCharacterClassData;

/**
 * 
 */
UCLASS()
class MAGE_API AMageGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	TObjectPtr<UCharacterClassData> CharacterClassData;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	TObjectPtr<UAbilityData> AbilityData;
};
