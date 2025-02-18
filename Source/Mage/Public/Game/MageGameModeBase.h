// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MageGameModeBase.generated.h"

class UMVVM_LoadSlot;
class ULoadScreenSaveGame;
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
	/** Data */
	
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	TObjectPtr<UCharacterClassData> CharacterClassData;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	TObjectPtr<UAbilityData> AbilityData;

	/** Save Game */
	void SaveSlotData(UMVVM_LoadSlot* LoadSlotViewModel, int32 InSlotIndex);
	ULoadScreenSaveGame* GetSaveGameObject(UMVVM_LoadSlot* LoadSlotViewModel, int32 InSlotIndex);
	static void DeleteSaveGameData(const FString& InSlotName, int32 InSlotIndex);
	void TraveToMap(UMVVM_LoadSlot* LoadSlotViewModel);
	
	UPROPERTY(EditDefaultsOnly, Category = "SaveGame")
	TSubclassOf<ULoadScreenSaveGame> LoadScreenSaveGameClass;

	UPROPERTY(EditDefaultsOnly, Category = "SaveGame")
	FString DefaultMapName;
	
	UPROPERTY(EditDefaultsOnly, Category = "SaveGame")
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly, Category = "SaveGame")
	TMap<FString, TSoftObjectPtr<UWorld>> GameMaps;
};
