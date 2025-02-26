// Copyright AFstz.


#include "MageAssetManager.h"
#include "MageGameplayTags.h"

UMageAssetManager& UMageAssetManager::Get()
{
	check(GEngine);
	
	UMageAssetManager* MageAssetManager = Cast<UMageAssetManager>(GEngine->AssetManager);
	return *MageAssetManager;
}

void UMageAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FMageGameplayTags::InitNativeGameplayTags();
}