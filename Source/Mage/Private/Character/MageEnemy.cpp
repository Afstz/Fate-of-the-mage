// Copyright AFstz.


#include "Character/MageEnemy.h"
#include "AbilitySystem/MageAbilitySystemComponent.h"
#include "AbilitySystem/MageAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "Mage/Mage.h"
#include "UI/Widget/MageUserWidget.h"

AMageEnemy::AMageEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UMageAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UMageAttributeSet>(TEXT("AttributeSet"));

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	HealthBar->SetupAttachment(RootComponent);
}
void AMageEnemy::BeginPlay()
{
	Super::BeginPlay();

	check(AbilitySystemComponent);
	InitAbilityActorInfo();
	InitHealthBar();
}

void AMageEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UMageAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoIsSet();
	InitDefaultAttributes();
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

