// Copyright AFstz.


#include "Character/MageCharacte.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/MagePlayerState.h"

AMageCharacte::AMageCharacte()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	// 启用角色移动约束到特定的平面
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// 关闭控制器来控制角色
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
}

void AMageCharacte::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init Ability Actor Info in the Server.
	InitAbilityActorInfo();
}

void AMageCharacte::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init Ability Actor Info in the Client.
	InitAbilityActorInfo();
	
}

void AMageCharacte::InitAbilityActorInfo()
{
	AMagePlayerState* MagePlayerState = GetPlayerState<AMagePlayerState>();
	check(MagePlayerState);
	AbilitySystemComponent = MagePlayerState->GetAbilitySystemComponent();
	AttributeSet = MagePlayerState->GetAttributeSet();
	AbilitySystemComponent->InitAbilityActorInfo(MagePlayerState, this);
}
