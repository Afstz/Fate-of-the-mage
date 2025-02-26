// Copyright AFstz.


#include "Game/MageGameInstance.h"

UMageGameInstance* UMageGameInstance::GetMageGameInstance()
{
	UMageGameInstance* MageGameInstance = nullptr;
	if (GEngine)
	{
		FWorldContext* WorldContext = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
		MageGameInstance = Cast<UMageGameInstance>(WorldContext->OwningGameInstance);
	}
	return MageGameInstance;
}