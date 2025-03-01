// Copyright AFstz.


#include "Actor/EnemySpawnTrigger.h"

#include "Actor/EnemySpawnPoint.h"
#include "Components/BoxComponent.h"
#include "Interface/PlayerInterface.h"

AEnemySpawnTrigger::AEnemySpawnTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxTrigger->SetupAttachment(GetRootComponent());

	BoxTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxTrigger->SetCollisionObjectType(ECC_WorldStatic);
	BoxTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AEnemySpawnTrigger::LoadActor_Implementation()
{
	// 再次加载地图时，摧毁这个触发器
	if (bTriggered)
	{
		Destroy();
	}
}

void AEnemySpawnTrigger::BeginPlay()
{
	Super::BeginPlay();

	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawnTrigger::OnBoxBeginOverlap);
}

void AEnemySpawnTrigger::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) && !OtherActor->Implements<UPlayerInterface>()) return;
	
	bTriggered = true;
	// 依次遍历生成敌人
	for (AEnemySpawnPoint* EnemySpawnPoint : EnemySpawnPoints)
	{
		if (!IsValid(EnemySpawnPoint)) return;
		
		EnemySpawnPoint->SpawnEnemy();
	}
	BoxTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
