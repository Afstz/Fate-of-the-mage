// Copyright AFstz.


#include "Character/MageCharacte.h"
#include "AbilitySystemComponent.h"
#include "MageGameplayTags.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/MageAbilitySystemComponent.h"
#include "AbilitySystem/MageAbilitySystemLibrary.h"
#include "AbilitySystem/MageAttributeSet.h"
#include "AbilitySystem/Data/AbilityData.h"
#include "AbilitySystem/Data/LevelUpData.h"
#include "Camera/CameraComponent.h"
#include "Game/MageGameInstance.h"
#include "Game/MageGameModeBase.h"
#include "Game/MageSaveGame.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
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

	LoadGameProgress();
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
	MagePlayerState->AddToLevel(InLevel, true);

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

void AMageCharacte::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	AMagePlayerController* MagePC = Cast<AMagePlayerController>(GetController());
	check(MagePC)
	MagePC->ShowMagicCircle(DecalMaterial);
	MagePC->bShowMouseCursor = false;
}

void AMageCharacte::HideMagicCircle_Implementation()
{
	AMagePlayerController* MagePC = Cast<AMagePlayerController>(GetController());
	check(MagePC)
	MagePC->HideMagicCircle();
	MagePC->bShowMouseCursor = true;
}

void AMageCharacte::SaveGameProgress_Implementation(const FName& CheckPointTag)
{
	AMageGameModeBase* MageGameModeBase = Cast<AMageGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (MageGameModeBase)
	{
		MageGameModeBase->SaveWorldState(GetWorld()); // 保存世界的Actors状态信息
		
		UMageSaveGame* MageSaveGame = MageGameModeBase->GetSaveGameObjectByGameInstance(UMageGameInstance::GetMageGameInstance());
		AMagePlayerState* MagePlayerState = GetPlayerState<AMagePlayerState>();
		if (!MagePlayerState && !IsValid(MageSaveGame)) return;
		
		// 保存玩家属性数据
		MageSaveGame->Level = MagePlayerState->GetPlayerLevel();
		MageSaveGame->XP = MagePlayerState->GetPlayerXP();
		MageSaveGame->AttributePoints = MagePlayerState->GetAttributePoints();
		MageSaveGame->SkillPoints = MagePlayerState->GetSkillPoints();
		MageSaveGame->Strength = UMageAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
		MageSaveGame->Intelligence = UMageAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
		MageSaveGame->Resilience = UMageAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
		MageSaveGame->Vigor = UMageAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());
		MageSaveGame->bFirstSaveGame = false;
		UMageGameInstance::GetMageGameInstance()->PlayerStartTag = CheckPointTag; // 保存当前存档点Tag

		// 保存玩家技能数据
		UMageAbilitySystemComponent* MageASC = Cast<UMageAbilitySystemComponent>(GetAbilitySystemComponent());
		UAbilityData* AbilityData = UMageAbilitySystemLibrary::GetAbilityData(this);
		FForeachAbilitiesSignature ForeachAbilities;
		
		ForeachAbilities.BindLambda([this, MageASC, AbilityData, MageSaveGame](const FGameplayAbilitySpec& AbilitySpec)
		{
			const FGameplayTag& AbilityTag = MageASC->GetAbilityTagFromSpec(AbilitySpec);
			FMageAbilityData FoundAbilityData = AbilityData->FindAbilityDataForTag(AbilityTag);
			
			FSavedAbilityData SavedAbilityData;
			SavedAbilityData.AbilityClass = FoundAbilityData.AbilityClass;
			SavedAbilityData.AbilityLevel = AbilitySpec.Level;
			SavedAbilityData.AbilityTag = FoundAbilityData.AbilityTag;
			SavedAbilityData.AbilityInputTag = MageASC->GetInputTagFromSpec(AbilitySpec);
			SavedAbilityData.StatusTag = MageASC->GetStatusTagFromSpec(AbilitySpec);
			SavedAbilityData.AbilityType = FoundAbilityData.AbilityType;
			
			MageSaveGame->SavedAbilitiesData.AddUnique(SavedAbilityData);
		});
		
		////////////////////////////////////
		// BUG: 在技能激活状态保存的时候会有BUG
		////////////////////////////////////

		MageASC->ForeachAbilitiesExecute(ForeachAbilities);
		
		MageGameModeBase->SaveDataInGameProgress(MageSaveGame);
	}
}

void AMageCharacte::LoadGameProgress()
{
	// PS.服务器上执行
	
	AMageGameModeBase* MageGameModeBase = Cast<AMageGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (MageGameModeBase)
	{
		MageGameModeBase->LoadWorldState(GetWorld()); // 加载世界的Actors状态信息
		
		UMageSaveGame* MageSaveGame = MageGameModeBase->GetSaveGameObjectByGameInstance(UMageGameInstance::GetMageGameInstance());
		AMagePlayerState* MagePlayerState = GetPlayerState<AMagePlayerState>();
		if (!MagePlayerState && !IsValid(MageSaveGame)) return;

		if (MageSaveGame->bFirstSaveGame)
		{
			// 第一次初始化(自动复制到客户端)
			InitDefaultAttributes();
			AddCharacterAbilites(); 
		}
		else
		{
			// 继承之前玩家数据
			MagePlayerState->SetLevel(MageSaveGame->Level, false);
			MagePlayerState->SetXP(MageSaveGame->XP);
			MagePlayerState->SetAttributePoints(MageSaveGame->AttributePoints);
			MagePlayerState->SetSkillPoints(MageSaveGame->SkillPoints);
			// 继承之间的属性
			UMageAbilitySystemLibrary::InitAttributesFromSaveGame(this, GetAbilitySystemComponent(), MageSaveGame);
			// 继承之前的技能
			UMageAbilitySystemComponent* MageASC = Cast<UMageAbilitySystemComponent>(GetAbilitySystemComponent());
			if (MageASC)
			{
				MageASC->AddCharacterAbilitiesFromSaveGame(MageSaveGame);
			}
		}
	}
}

void AMageCharacte::BeginPlay()
{
	Super::BeginPlay();
}

void AMageCharacte::OnRep_Stunned(bool OldStunned)
{
	// 客户端眩晕状态禁止的操作
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
