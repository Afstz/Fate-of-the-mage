// Copyright AFstz.


#include "Character/MageEnemy.h"
#include "AbilitySystem/MageAbilitySystemComponent.h"
#include "AbilitySystem/MageAbilitySystemLibrary.h"
#include "AbilitySystem/MageAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "Mage/Mage.h"
#include "UI/Widget/MageUserWidget.h"
#include "MageGameplayTags.h"
#include "AI/MageAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AMageEnemy::AMageEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UMageAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UMageAttributeSet>(TEXT("AttributeSet"));

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	HealthBar->SetupAttachment(RootComponent);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void AMageEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	MageAIController = Cast<AMageAIController>(NewController);
	// AIController会自带一个BBComp,需要手动初始化它
	MageAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	MageAIController->RunBehaviorTree(BehaviorTree);
	MageAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	MageAIController->GetBlackboardComponent()->SetValueAsBool(FName("MeleeCharacter"), CharacterClass == ECharacterClass::Warrior);
}

void AMageEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	
	InitAbilityActorInfo();
	
	InitHealthBar();

	if (HasAuthority())
	{
		UMageAbilitySystemLibrary::GiveCharacterAbilities(this, AbilitySystemComponent, CharacterClass);
	}
}

void AMageEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UMageAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoIsSet();
	if (HasAuthority())
	{
		InitDefaultAttributes();
	}
}

void AMageEnemy::InitDefaultAttributes()
{
	UMageAbilitySystemLibrary::InitCharacterDefaultAttributes(this, GetAbilitySystemComponent(), CharacterClass, Level);
}

void AMageEnemy::InitHealthBar()
{
	if (UMageUserWidget* EnemyWidget = Cast<UMageUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		EnemyWidget->SetWidgetController(this);
		const UMageAttributeSet* EnemeyAttributeSet = CastChecked<UMageAttributeSet>(AttributeSet);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EnemeyAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& HealthData)
			{
				HealthChanged.Broadcast(HealthData.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EnemeyAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& HealthData)
			{
				MaxHealthChanged.Broadcast(HealthData.NewValue);
			}
		);
		HealthChanged.Broadcast(EnemeyAttributeSet->GetHealth());
		MaxHealthChanged.Broadcast(EnemeyAttributeSet->GetMaxHealth());
		
		// 注册接收到指定Tag调用回调函数
		AbilitySystemComponent->RegisterGameplayTagEvent(
			FMageGameplayTags::Get().Effects_HitReact,
			EGameplayTagEventType::NewOrRemoved).AddUObject(
			this, &ThisClass::HitReactCallBack);
		
	}
}

void AMageEnemy::HitReactCallBack(const FGameplayTag HitReactTag, int32 NewCount)
{
	bHitReact = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReact ? 0 : BaseWalkSpeed;
	
	if (MageAIController) // 因为客户端上没有AIController所以需要判断
	{
		MageAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsHitReacting"), bHitReact);
	}
}

void AMageEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AMageEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

AActor* AMageEnemy::GetFacingTarget_Implementation() const
{
	return FacingTarget;
}

void AMageEnemy::SetFacingTarget_Implementation(AActor* InFacingTarget)
{
	FacingTarget = InFacingTarget;
}

FVector AMageEnemy::GetSocketLocationByStruct_Implementation(const FTaggedMontage& TaggedMontage) const
{
	// 通过传进来的结构体判断属于那种类型的攻击，根据类型播放蒙太奇动画并接收指定标签事件
	
	if (TaggedMontage.CombatSocketTag.MatchesTagExact(FMageGameplayTags::Get().CombatSocket_Weapon))
	{
		// 武器类型
		check(Weapon);
		return Weapon->GetSocketLocation(TaggedMontage.CombatSocketName);
	}
	else
	{
		// 无武器
		return GetMesh()->GetSocketLocation(TaggedMontage.CombatSocketName);
	}
}

TArray<FTaggedMontage> AMageEnemy::GetTaggedMontage_Implementation() const
{
	return AttackMontages;
}

FTaggedMontage AMageEnemy::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage& TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageEventTag.MatchesTagExact(MontageTag))
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

void AMageEnemy::MultiHiddenWidget_Implementation()
{
	HealthBar->SetVisibility(false);
}

void AMageEnemy::Die()
{
	if (MageAIController)
	{
		MageAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	}
	SetLifeSpan(LifeSpan);
	MultiHiddenWidget();
	Super::Die();
}


