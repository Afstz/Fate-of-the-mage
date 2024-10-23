// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MageGameModeBase.generated.h"

class UCharacterClassData;

/**
 * 
 */
UCLASS()
class MAGE_API AMageGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassData> CharacterClassData;
};
