// Copyright AFstz.


#include "Character/MageCharacte.h"
#include "AbilitySystemComponent.h"
#include "MageGameplayTags.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/MageAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpData.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/MagePlayerController.h"
#include "Player/MagePlayerState.h"
#include "UI/HUD/MageHUD.h"

AMageCharacte::AMageCharacte()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Comp"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Comp"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LevelUp Niagara Comp"));
	LevelUpNiagaraComponent->SetupAttachment(RootComponent);
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	// 启用角色移动约束到特定的平面
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// 禁止控制器影响角色和相机的旋转
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CharacterClass = ECharacterClass::Mage;
}

void AMageCharacte::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// Init Ability Actor Info in the Server.

	InitAbilityActorInfo();
	
	InitDefaultAttributes();
	AddCharacterAbilites(); // 在服务器赋予技能
}

void AMageCharacte::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init Ability Actor Info in the Client.
	InitAbilityActorInfo();
}

int32 AMageCharacte::GetCharacterLevel_Implementation() const
{
	AMagePlayerState* MagePlayerState = Cast<AMagePlayerState>(GetPlayerState());
	check(MagePlayerState);
	return MagePlayerState->GetPlayerLevel();
}

int32 AMageCharacte::GetXP_Implementation() const
{
	AMagePlayerState* MagePlayerState = Cast<AMagePlayerState>(GetPlayerState());
	check(MagePlayerState);
	return MagePlayerState->GetPlayerXP();
}

int32 AMageCharacte::GetAttributePointReward_Implementation(const int32 InLevel) const
{
	AMagePlayerState* MagePlayerState = Cast<AMagePlayerState>(GetPlayerState());
	check(MagePlayerState);
	return MagePlayerState->LevelUpData->LevelUpDatas[InLevel].AttributePointReward;
}

int32 AMageCharacte::GetSkillPointReward_Implementation(const int32 InLevel) const
{
	AMagePlayerState* MagePlayerState = Cast<AMagePlayerState>(GetPlayerState());
	check(MagePlayerState);
	return MagePlayerState->LevelUpData->LevelUpDatas[InLevel].SkillPointReward;
}

int32 AMageCharacte::GetAttributePoint_Implementation() const
{
	AMagePlayerState* MagePlayerState = Cast<AMagePlayerState>(GetPlayerState());
	check(MagePlayerState);
	return MagePlayerState->GetAttributePoints();
}

int32 AMageCharacte::GetSkillPoint_Implementation() const
{
	AMagePlayerState* MagePlayerState = Cast<AMagePlayerState>(GetPlayerState());
	check(MagePlayerState);
	return MagePlayerState->GetSkillPoints();
}

void AMageCharacte::AddToXP_Implementation(int32 InXP)
{
	AMagePlayerState* MagePlayerState = Cast<AMagePlayerState>(GetPlayerState());
	check(MagePlayerState);
	MagePlayerState->AddToXP(InXP);
}

void AMageCharacte::AddToLevel_Implementation(int32 InLevel)
{
	AMagePlayerState* MagePlayerState = Cast<AMagePlayerState>(GetPlayerState());
	check(MagePlayerState);
	MagePlayerState->AddToLevel(InLevel);

	UMageAbilitySystemComponent* MageASC = CastChecked<UMageAbilitySystemComponent>(GetAbilitySystemComponent());
	MageASC->UpdateAbilityStatuses(MagePlayerState->GetPlayerLevel()); // 更新符合等级的技能状态
}

void AMageCharacte::AddToAttributePoint_Implementation(int32 InAttributePoint)
{
	AMagePlayerState* MagePlayerState = Cast<AMagePlayerState>(GetPlayerState());
	check(MagePlayerState);
	MagePlayerState->AddToAttributePoints(InAttributePoint);
}

void AMageCharacte::AddToSkillPoint_Implementation(int32 InSkillPoint)
{
	AMagePlayerState* MagePlayerState = Cast<AMagePlayerState>(GetPlayerState());
	check(MagePlayerState);
	MagePlayerState->AddToSkillPoints(InSkillPoint);
}

void AMageCharacte::BeginPlay()
{
	Super::BeginPlay();
}

void AMageCharacte::OnRep_Stunned(bool OldStunned)
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		const FMageGameplayTags& MageGameplayTags = FMageGameplayTags::Get();
		FGameplayTagContainer BlockTags;
		BlockTags.AddTag(MageGameplayTags.Block_Player_InputPressed);
		BlockTags.AddTag(MageGameplayTags.Block_Player_InputHeld);
		BlockTags.AddTag(MageGameplayTags.Block_Player_InputReleased);
		BlockTags.AddTag(MageGameplayTags.Block_Player_AutoRun);
		if (bStunned)
		{
			ASC->AddLooseGameplayTags(BlockTags);
		}
		else
		{
			ASC->RemoveLooseGameplayTags(BlockTags);
		}
	}
}

void AMageCharacte::OnStunTagChanged(const FGameplayTag StunTag, int32 NewCount)
{
	Super::OnStunTagChanged(StunTag, NewCount);
	
}

void AMageCharacte::LevelUp_Implementation()
{
	MulticastLevelUpEffect();
}

void AMageCharacte::MulticastLevelUpEffect_Implementation()
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocaiton = Camera->GetComponentLocation();
		const FVector EffectLocaiton = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCamera = (CameraLocaiton - EffectLocaiton).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCamera);
		LevelUpNiagaraComponent->Activate(true);
	}
}

int32 AMageCharacte::FindLevelForXP_Implementation(const int32 InXP)
{
	AMagePlayerState* MagePlayerState = GetPlayerState<AMagePlayerState>();
	check(MagePlayerState);
	return MagePlayerState->LevelUpData->FindLevelForXP(InXP);
}


void AMageCharacte::InitAbilityActorInfo()
{
	AMagePlayerState* MagePlayerState = GetPlayerState<AMagePlayerState>();
	check(MagePlayerState);
	AbilitySystemComponent = MagePlayerState->GetAbilitySystemComponent();
	AttributeSet = MagePlayerState->GetAttributeSet(); 
	AbilitySystemComponent->InitAbilityActorInfo(MagePlayerState, this);
	Cast<UMageAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoIsSet(); // 绑定效果应用回调
	ASCRegisteredDelegate.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FMageGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnStunTagChanged);
	
	if (AMagePlayerController* PlayerController = Cast<AMagePlayerController>(GetController()))
	{
		if (AMageHUD* MageHUD = Cast<AMageHUD>(PlayerController->GetHUD()))
		{
			MageHUD->InitOverlay(PlayerController, MagePlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}
