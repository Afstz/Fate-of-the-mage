// Copyright AFstz.


#include "MageCharacterBase.h"

// Sets default values
AMageCharacterBase::AMageCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMageCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMageCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMageCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

