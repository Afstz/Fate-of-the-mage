// Copyright AFstz.


#include "Actor/MagicCircle.h"
#include "Components/DecalComponent.h"

// Sets default values
AMagicCircle::AMagicCircle()
{
	PrimaryActorTick.bCanEverTick = true;

	MagicCircleDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("MagicCircleDecalComponent"));
	SetRootComponent(MagicCircleDecalComponent);
}

void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

