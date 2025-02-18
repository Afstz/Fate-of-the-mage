// Copyright AFstz.


#include "Game/MageGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "Game/LoadScreenSaveGame.h"

void AMageGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlotViewModel, int32 InSlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlotViewModel->GetLoadSlotName(), InSlotIndex)) 
	{
		// 存档存在就删除数据重新保存
		UGameplayStatics::DeleteGameInSlot(LoadSlotViewModel->GetLoadSlotName(), InSlotIndex);
	}
	
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	LoadScreenSaveGame->CharacterName = LoadSlotViewModel->GetCharacterName();
	LoadScreenSaveGame->MapName = LoadSlotViewModel->GetMapName();
	LoadScreenSaveGame->LoadSlotStatus = LoadSlotViewModel->LoadSlotStatus;
	
	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlotViewModel->GetLoadSlotName(), InSlotIndex);
}

ULoadScreenSaveGame* AMageGameModeBase::GetSaveGameObject(UMVVM_LoadSlot* LoadSlotViewModel, int32 InSlotIndex)
{
	USaveGame* SaveGameObject = nullptr;
	
	if (UGameplayStatics::DoesSaveGameExist(LoadSlotViewModel->GetLoadSlotName(), InSlotIndex)) 
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(LoadSlotViewModel->GetLoadSlotName(), InSlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
	
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);

	return LoadScreenSaveGame;
}

void AMageGameModeBase::DeleteSaveGameData(const FString& InSlotName, int32 InSlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(InSlotName, InSlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(InSlotName, InSlotIndex);
	}
}

void AMageGameModeBase::TraveToMap(UMVVM_LoadSlot* LoadSlotViewModel)
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, GameMaps.FindChecked(LoadSlotViewModel->GetMapName()));
}