// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "MageAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class MAGE_API UMageAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static UMageAssetManager& Get();
	virtual void StartInitialLoading() override;
};
