// Copyright AFstz.


#include "Actor/CheckPoint/CheckPoint.h"

#include "Components/SphereComponent.h"
#include "Game/MageGameModeBase.h"
#include "Interface/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Mage/Mage.h"

ACheckPoint::ACheckPoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	CheckPointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CheckPointMesh"));
	CheckPointMesh->SetupAttachment(RootComponent);
	CheckPointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckPointMesh->SetCollisionResponseToAllChannels(ECR_Block);
	CheckPointMesh->SetCustomDepthStencilValue(CustomDepthStencilValue);
	CheckPointMesh->MarkRenderStateDirty(); // 强制更新渲染

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(CheckPointMesh);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	DestinationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DestinationComponent"));
	DestinationComponent->SetupAttachment(RootComponent);
}

void ACheckPoint::LoadActor_Implementation()
{
	// 反序列后处理
	if (bReached)
	{
		HandleCheckPointGlow();
	}
}

void ACheckPoint::HighlightActor_Implementation()
{
	if (!bReached)
	{
		CheckPointMesh->SetRenderCustomDepth(true);
	}
}

void ACheckPoint::UnHighlightActor_Implementation()
{
	CheckPointMesh->SetRenderCustomDepth(false);
}

void ACheckPoint::MoveToLocation_Implementation(FVector& OutDestination)
{
	OutDestination = DestinationComponent->GetComponentLocation();
}

void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlap);
}

void ACheckPoint::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bool bIsServer = OtherActor->HasAuthority();
	
	if (bIsServer && OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;
		if (AMageGameModeBase* MageGameModeBase = Cast<AMageGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			MageGameModeBase->SaveWorldState(GetWorld()); // 保存世界的Actors状态信息
		}
		IPlayerInterface::Execute_SaveGameProgress(OtherActor, PlayerStartTag); // 保存进度
		HandleCheckPointGlow(); // 存档点变亮
	}
}

void ACheckPoint::HandleCheckPointGlow()
{
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(CheckPointMesh->GetMaterial(0), this);
	CheckPointMesh->SetMaterial(0, DynamicMaterialInstance);
	CheckPointGlow(DynamicMaterialInstance);
}