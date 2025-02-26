// Copyright AFstz.


#include "UI/ViewModel/MVVM_LoadScreen.h"
#include "Game/MageSaveGame.h"
#include "Game/MageGameInstance.h"
#include "Game/MageGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlotViewModels()
{
	LoadSlotViewModel_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlotViewModel_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlotViewModel_0->SlotIndex = 0;
	LoadSlotViewModels.Add(0, LoadSlotViewModel_0);
	
	LoadSlotViewModel_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlotViewModel_1->SetLoadSlotName(FString("LoadSlot_1"));
	LoadSlotViewModel_1->SlotIndex = 1;
	LoadSlotViewModels.Add(1, LoadSlotViewModel_1);
	
	LoadSlotViewModel_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlotViewModel_2->SetLoadSlotName(FString("LoadSlot_2"));
	LoadSlotViewModel_2->SlotIndex = 2;
	LoadSlotViewModels.Add(2, LoadSlotViewModel_2);
}

void UMVVM_LoadScreen::LoadSaveData()
{
	AMageGameModeBase* MageGameModeBase = Cast<AMageGameModeBase>(UGameplayStatics::GetGameMode(this));

	// 遍历LoadSlotVMs加载数据
	for (auto& [InSlotIndex, InLoadSlotViewModel] : LoadSlotViewModels)
	{
		UMageSaveGame* MageSaveGame = MageGameModeBase->GetSaveGameObjectByName(InLoadSlotViewModel->GetLoadSlotName(), InSlotIndex);
		InLoadSlotViewModel->LoadSlotStatus = MageSaveGame->LoadSlotStatus;
		InLoadSlotViewModel->SetCharacterName(MageSaveGame->CharacterName);
		InLoadSlotViewModel->SetPlayerLevel(MageSaveGame->Level);
		InLoadSlotViewModel->SetMapName(MageSaveGame->MapName);
		InLoadSlotViewModel->PlayerStartTag = MageSaveGame->PlayerStartTag;
		InLoadSlotViewModel->InitializeSlot();
	}
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlotViewModels.FindChecked(Index);
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 InSlotIndex)
{
	LoadSlotViewModels[InSlotIndex]->SetWidgetSwitcherIndex.Broadcast(1); // 切换到创建角色Widget
}

void UMVVM_LoadScreen::CreateCharacterButtonPressed(int32 InSlotIndex, const FString& CharacterName)
{
	AMageGameModeBase* MageGameModeBase = Cast<AMageGameModeBase>(UGameplayStatics::GetGameMode(this));

	LoadSlotViewModels[InSlotIndex]->SetCharacterName(CharacterName);
	LoadSlotViewModels[InSlotIndex]->SetPlayerLevel(1);
	LoadSlotViewModels[InSlotIndex]->SetMapName(MageGameModeBase->DefaultMapName);
	LoadSlotViewModels[InSlotIndex]->LoadSlotStatus = ELoadSlotStatus::Taken;
	LoadSlotViewModels[InSlotIndex]->PlayerStartTag = MageGameModeBase->DefaultPlayerStartTag;
	
	MageGameModeBase->SaveSlotData(LoadSlotViewModels[InSlotIndex], InSlotIndex); // 保存创建的角色名
	
	LoadSlotViewModels[InSlotIndex]->InitializeSlot(); // 切换Slot
}

void UMVVM_LoadScreen::LoadArchiveButtonPressed(int32 InSlotIndex)
{
	LoadArchiveSelected.Broadcast(); // 广播选中状态启用Start/Delete Button
	SelectedSlotVM = LoadSlotViewModels[InSlotIndex];
	for (auto& [SlotIndex, InLoadSlotViewModel] : LoadSlotViewModels)
	{
		// 相同即选中状态，其他按钮恢复初始状态
		if (SlotIndex == InSlotIndex)
		{
			InLoadSlotViewModel->EnableLoadArchiveButton.Broadcast(false);
		}
		else
		{
			InLoadSlotViewModel->EnableLoadArchiveButton.Broadcast(true);
		}
	}
}

void UMVVM_LoadScreen::StartGameButtonPressed()
{
	AMageGameModeBase* MageGameModeBase = Cast<AMageGameModeBase>(UGameplayStatics::GetGameMode(this));
	
	if (IsValid(SelectedSlotVM))
	{
		UMageGameInstance::GetMageGameInstance()->PlayerStartTag = SelectedSlotVM->PlayerStartTag;
		UMageGameInstance::GetMageGameInstance()->LoadSlotName = SelectedSlotVM->GetLoadSlotName();
		UMageGameInstance::GetMageGameInstance()->LoadSlotIndex = SelectedSlotVM->SlotIndex;
		
		MageGameModeBase->TraveToMap(SelectedSlotVM); // 传送到当前插槽保存的地图
	}
}

void UMVVM_LoadScreen::DialogConfirmButtonPressed()
{
	if (IsValid(SelectedSlotVM))
	{
		AMageGameModeBase::DeleteSaveGameData(SelectedSlotVM->GetLoadSlotName(), SelectedSlotVM->SlotIndex);
		SelectedSlotVM->LoadSlotStatus = ELoadSlotStatus::Vacant;
		SelectedSlotVM->InitializeSlot(); // 初始化Slot
		SelectedSlotVM->EnableLoadArchiveButton.Broadcast(true); // 启用存档按钮
	}
}

void UMVVM_LoadScreen::SetLoadScreenName(const FString& InLoadScreenName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadScreenName, InLoadScreenName);
}