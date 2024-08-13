// Copyright AFstz.


#include "Character/MageCharacterBase.h"

AMageCharacterBase::AMageCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void AMageCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}


