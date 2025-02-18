// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "LoadScreenSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class MAGE_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// 存档插槽名 
	UPROPERTY()
	FString LoadSlotName = FString(); 

	// 存档插槽索引
	UPROPERTY()
	int32 SlotIndex = 0;

	// 存档保存的角色名称
	UPROPERTY()
	FString CharacterName = FString();

	// 存档保存的地图名称
	UPROPERTY()
	FString MapName = FString();

	// 当前插槽的状态
	UPROPERTY()
	TEnumAsByte<ELoadSlotStatus> LoadSlotStatus = ELoadSlotStatus::Vacant;
};
