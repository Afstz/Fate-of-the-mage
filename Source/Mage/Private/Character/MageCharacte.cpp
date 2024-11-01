// Copyright AFstz.


#include "Character/MageCharacte.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/MageAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/MagePlayerController.h"
#include "Player/MagePlayerState.h"
#include "UI/HUD/MageHUD.h"

AMageCharacte::AMageCharacte()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	// 启用角色移动约束到特定的平面
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// 禁止控制器影响角色和相机的旋转
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
}

void AMageCharacte::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// Init Ability Actor Info in the Server.
	InitAbilityActorInfo();
	
	AddCharacterAbilites(); // 在服务器赋予技能
	InitDefaultAttributes();
	
}

void AMageCharacte::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init Ability Actor Info in the Client.
	InitAbilityActorInfo();
}

int32 AMageCharacte::GetCharacterLevel() const
{
	AMagePlayerState* MagePlayerState = Cast<AMagePlayerState>(GetPlayerState());
	check(MagePlayerState);
	return MagePlayerState->GetPlayerLevel();
}

void AMageCharacte::InitAbilityActorInfo()
{
	AMagePlayerState* MagePlayerState = GetPlayerState<AMagePlayerState>();
	check(MagePlayerState);
	AbilitySystemComponent = MagePlayerState->GetAbilitySystemComponent();
	AttributeSet = MagePlayerState->GetAttributeSet(); 
	AbilitySystemComponent->InitAbilityActorInfo(MagePlayerState, this);
	Cast<UMageAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoIsSet(); // 绑定效果应用回调

	if (AMagePlayerController* PlayerController = Cast<AMagePlayerController>(GetController()))
	{
		if (AMageHUD* MageHUD = Cast<AMageHUD>(PlayerController->GetHUD()))
		{
			MageHUD->InitOverlay(PlayerController, MagePlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}
