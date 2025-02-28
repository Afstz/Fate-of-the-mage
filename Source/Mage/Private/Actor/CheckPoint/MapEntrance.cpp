// Copyright AFstz.


#include "Actor/CheckPoint/MapEntrance.h"
#include "Components/SphereComponent.h"
#include "Game/MageGameModeBase.h"
#include "Interface/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Sphere->SetupAttachment(DestinationComponent);
}

void AMapEntrance::LoadActor_Implementation()
{
	// 不做处理
}

void AMapEntrance::HighlightActor_Implementation()
{
	CheckPointMesh->SetRenderCustomDepth(true);
}

void AMapEntrance::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bool bIsServer = OtherActor->HasAuthority();
	
	if (bIsServer && OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;
		if (AMageGameModeBase* MageGameModeBase = Cast<AMageGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			// 保存世界的Actors状态信息,并且更新新地图名字
			MageGameModeBase->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName()); 
		}
		// 保存进度
		IPlayerInterface::Execute_SaveGameProgress(OtherActor, DestinationPlayerStart); 

		// 切换到目的地图
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), DestinationMap);
	}
}