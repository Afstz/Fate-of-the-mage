// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MageGameInstance.generated.h"

/**
 *  自定义的全局单例类，用于保存游戏数据
 */
UCLASS()
class MAGE_API UMageGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static UMageGameInstance* GetMageGameInstance();

	// 玩家起始点标签
	UPROPERTY()
	FName PlayerStartTag = FName("MainGame1PlayerStart1");

	// 存档插槽名
	UPROPERTY()
	FString LoadSlotName = FString("LoadSlot_0");

	// 存档插槽索引
	UPROPERTY()
	int32 LoadSlotIndex = 0;
};
