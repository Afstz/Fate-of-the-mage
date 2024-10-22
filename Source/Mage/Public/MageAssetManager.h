// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "MageAssetManager.generated.h"

/**
 *  资源管理单例类
 *	用于初始化自定义的资源
 */
UCLASS()
class MAGE_API UMageAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static UMageAssetManager& Get();
	virtual void StartInitialLoading() override;
};
