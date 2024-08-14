// Copyright AFstz.


#include "Character/MageCharacte.h"

#include "GameFramework/CharacterMovementComponent.h"

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