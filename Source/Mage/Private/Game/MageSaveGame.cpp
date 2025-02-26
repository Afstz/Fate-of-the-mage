// Copyright AFstz.


#include "Game/MageSaveGame.h"

FSavedWorld UMageSaveGame::GetSavedWorldByWorldName(const FString& InWorldName)
{
	for (const FSavedWorld& SavedWorld : SavedWorlds)
	{
		if (SavedWorld.MapAssetName == InWorldName)
		{
			return SavedWorld;
		}
	}
	return FSavedWorld();
}

bool UMageSaveGame::HasMap(const FString& InWorldName)
{
	for (const FSavedWorld& SavedWorld : SavedWorlds)
	{
		if (SavedWorld.MapAssetName == InWorldName)
		{
			return true;
		}
	}
	return false;
}