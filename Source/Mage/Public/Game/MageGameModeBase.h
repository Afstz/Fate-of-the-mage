// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MageGameModeBase.generated.h"

class ULootTiers;
class UMageGameInstance;
class UMVVM_LoadSlot;
class UMageSaveGame;
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

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	TObjectPtr<ULootTiers> LootTiers;

	/** Save Game */
	void SaveSlotData(UMVVM_LoadSlot* LoadSlotViewModel, int32 InSlotIndex); 
	void SaveDataInGameProgress(UMageSaveGame* MageSaveGame);
	UMageSaveGame* GetSaveGameObjectByName(const FString& InSlotName, int32 InSlotIndex);
	UMageSaveGame* GetSaveGameObjectByGameInstance(UMageGameInstance* MageGameInstance);
	static void DeleteSaveGameData(const FString& InSlotName, int32 InSlotIndex);
	void TraveToMap(UMVVM_LoadSlot* LoadSlotViewModel);
	void SaveWorldState(UWorld* World, const FString& DestinationMapAssetName = FString(""));
	void LoadWorldState(UWorld* World);

	/** Player */
	void PlayerRespawn(ACharacter* DiedCharacter, AController* DiedController);
	
	UPROPERTY(EditDefaultsOnly, Category = "SaveGame")
	TSubclassOf<UMageSaveGame> MageSaveGameClass;

	// 默认地图名字
	UPROPERTY(EditDefaultsOnly, Category = "SaveGame")
	FString DefaultMapName;

	// 默认地图起始点Tag
	UPROPERTY(EditDefaultsOnly, Category = "SaveGame")
	FName DefaultPlayerStartTag;

	// 软对象引用,使用时才会加载,减少内存占用
	UPROPERTY(EditDefaultsOnly, Category = "SaveGame")
	TSoftObjectPtr<UWorld> DefaultMap; 

	UPROPERTY(EditDefaultsOnly, Category = "SaveGame")
	TMap<FString, TSoftObjectPtr<UWorld>> GameMaps;
	FString GetMapNameByMapAssetName(const FString& MapAssetName);

protected:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
};
