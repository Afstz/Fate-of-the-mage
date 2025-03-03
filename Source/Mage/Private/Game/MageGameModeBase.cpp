// Copyright AFstz.


#include "Game/MageGameModeBase.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "Game/MageSaveGame.h"
#include "Game/MageGameInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "Interface/SaveInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void AMageGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlotViewModel, int32 InSlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlotViewModel->GetLoadSlotName(), InSlotIndex)) 
	{
		// 存档存在就删除数据重新保存
		UGameplayStatics::DeleteGameInSlot(LoadSlotViewModel->GetLoadSlotName(), InSlotIndex);
	}
	
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(MageSaveGameClass);
	UMageSaveGame* MageSaveGame = Cast<UMageSaveGame>(SaveGameObject);
	MageSaveGame->LoadSlotName = LoadSlotViewModel->GetLoadSlotName();
	MageSaveGame->SlotIndex = LoadSlotViewModel->SlotIndex;
	MageSaveGame->CharacterName = LoadSlotViewModel->GetCharacterName();
	MageSaveGame->MapName = LoadSlotViewModel->GetMapName();
	MageSaveGame->LoadSlotStatus = LoadSlotViewModel->LoadSlotStatus;
	MageSaveGame->PlayerStartTag = LoadSlotViewModel->PlayerStartTag;
	
	UGameplayStatics::SaveGameToSlot(MageSaveGame, LoadSlotViewModel->GetLoadSlotName(), InSlotIndex);
}

void AMageGameModeBase::SaveDataInGameProgress(UMageSaveGame* MageSaveGame)
{
	const FString InSlotName = UMageGameInstance::GetMageGameInstance()->LoadSlotName;
	const int32 InSlotIndex = UMageGameInstance::GetMageGameInstance()->LoadSlotIndex;
	MageSaveGame->PlayerStartTag = UMageGameInstance::GetMageGameInstance()->PlayerStartTag;
	
	UGameplayStatics::SaveGameToSlot(MageSaveGame, InSlotName, InSlotIndex);
}

UMageSaveGame* AMageGameModeBase::GetSaveGameObjectByName(const FString& InSlotName, int32 InSlotIndex)
{
	USaveGame* SaveGameObject = nullptr;
	
	if (UGameplayStatics::DoesSaveGameExist(InSlotName, InSlotIndex)) 
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(InSlotName, InSlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(MageSaveGameClass);
	}
	
	UMageSaveGame* MageSaveGame = Cast<UMageSaveGame>(SaveGameObject);

	return MageSaveGame;
}

UMageSaveGame* AMageGameModeBase::GetSaveGameObjectByGameInstance(UMageGameInstance* MageGameInstance)
{
	const FString InLoadSlotName = MageGameInstance->LoadSlotName;
	const int32 InLoadSlotIndex = MageGameInstance->LoadSlotIndex;

	return GetSaveGameObjectByName(InLoadSlotName, InLoadSlotIndex);
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

void AMageGameModeBase::SaveWorldState(UWorld* World, const FString& DestinationMapAssetName)
{
	FString WorldName = World->GetMapName();
	// 移除地图字符串前缀获取最原始的资产名
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	UMageGameInstance* MageGameInstance = UMageGameInstance::GetMageGameInstance();
	check(MageGameInstance);
	
	if (UMageSaveGame* MageSaveGame = GetSaveGameObjectByGameInstance(MageGameInstance))
	{
		if (DestinationMapAssetName != FString("")) // 切换到新地图
		{
			MageSaveGame->MapAssetName = DestinationMapAssetName;
			// 用于更新UI
			MageSaveGame->MapName = GetMapNameByMapAssetName(DestinationMapAssetName);
		}
			
		if (!MageSaveGame->HasMap(WorldName))
		{
			FSavedWorld SavedWorld; // 要保存的世界信息
			SavedWorld.MapAssetName = WorldName;
			MageSaveGame->SavedWorlds.AddUnique(SavedWorld);
		}
		FSavedWorld SavedWorld = MageSaveGame->GetSavedWorldByWorldName(WorldName);
		SavedWorld.SavedActors.Empty(); // 清除之前保存的Actor信息
		
		// 遍历这个世界所有的Actor寻找需要保存的Actor
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;
			
			FSavedActor SavedActor;
			SavedActor.ActorName = Actor->GetFName();
			SavedActor.ActorTransform = Actor->GetTransform();

			// 创建一个内存写入器，用于将数据直接写入内存中的字节数组
			FMemoryWriter MemoryWriter(SavedActor.SerializeBytes);
			
			// UE 提供的特殊存档类，将对象引用转换为字符串路径，反序列化时再通过路径重新查找对象。
			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
			// 标记此存档用于保存游戏,仅保存标记为 UPROPERTY(SaveGame) 的属性。
			Archive.ArIsSaveGame = true;
			// 将Actor的数据序列化到存档中。 
			Actor->Serialize(Archive);
			
			SavedWorld.SavedActors.AddUnique(SavedActor);
		}
		
		for (FSavedWorld& InSavedWorld : MageSaveGame->SavedWorlds)
		{
			if (InSavedWorld.MapAssetName == WorldName)
			{
				InSavedWorld = SavedWorld; // 覆盖
			}
		}
		SaveDataInGameProgress(MageSaveGame); // 保存
	}
}

void AMageGameModeBase::LoadWorldState(UWorld* World)
{
	FString WorldName = World->GetMapName();
	// 移除地图字符串前缀获取最原始的资产名
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	UMageGameInstance* MageGameInstance = UMageGameInstance::GetMageGameInstance();
	check(MageGameInstance);

	if (UMageSaveGame* MageSaveGame = GetSaveGameObjectByGameInstance(MageGameInstance))
	{
		if (!MageSaveGame->HasMap(WorldName)) return;
		
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;

			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;

			for (FSavedActor InSavedActor : MageSaveGame->GetSavedWorldByWorldName(WorldName).SavedActors)
			{
				if (InSavedActor.ActorName == Actor->GetFName())
				{
					if (ISaveInterface::Execute_ShouldLoadTransform(Actor)) // 判断Actor是否要变换位置
					{
						Actor->SetActorTransform(InSavedActor.ActorTransform);
					}

					// 内存读取器
					FMemoryReader MemoryReader(InSavedActor.SerializeBytes);
					// 将二进制流中的字符串路径解析为实际对象引用
					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;

					// 反序列化二进制字节转换为变量
					Actor->Serialize(Archive);

					ISaveInterface::Execute_LoadActor(Actor); // 执行Actor自定义的加载逻辑
				}
			}
		}
	}
}

FString AMageGameModeBase::GetMapNameByMapAssetName(const FString& MapAssetName)
{
	//MapName: 显示到UI的名字 MapAssetName: 地图资产名
	for (auto [MapName, Map] : GameMaps)
	{
		if (Map.ToSoftObjectPath().GetAssetName() == MapAssetName)
		{
			return MapName;
		}
	}
	return FString();
}

void AMageGameModeBase::PlayerRespawn(ACharacter* DiedCharacter, AController* DiedController)
{
	if (DiedCharacter)
	{
		DiedCharacter->Destroy();
	}
	if (DiedController)
	{
		TArray<AActor*>PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		AActor* SelectedStart = nullptr;
		if (PlayerStarts.Num() > 0)
		{
			for (AActor* Actor : PlayerStarts)
			{
				APlayerStart* PlayerStart = Cast<APlayerStart>(Actor);
				if (PlayerStart->PlayerStartTag == UMageGameInstance::GetMageGameInstance()->PlayerStartTag)
				{
					SelectedStart = PlayerStart;
					break;
				}
			}
		}
		RestartPlayerAtPlayerStart(DiedController, SelectedStart);
	}
}

AActor* AMageGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
	AActor* SelectedStart = nullptr;
	if (PlayerStarts.Num() > 1)
	{
		for (AActor* Actor : PlayerStarts)
		{
			APlayerStart* PlayerStart = Cast<APlayerStart>(Actor);
			if (PlayerStart->PlayerStartTag == UMageGameInstance::GetMageGameInstance()->PlayerStartTag)
			{
				SelectedStart = PlayerStart;
				break;
			}
		}
	}
	else if (PlayerStarts.Num() == 1)
	{
		SelectedStart = PlayerStarts[0];
	}
	return SelectedStart;
}