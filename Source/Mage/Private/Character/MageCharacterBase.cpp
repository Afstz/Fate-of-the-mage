// Copyright AFstz.


#include "Character/MageCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/MageAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Mage/Mage.h"

AMageCharacterBase::AMageCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AMageCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

FVector AMageCharacterBase::GetLocationByWeaponSocket() const
{
	check(Weapon);
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

void AMageCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMageCharacterBase::InitAbilityActorInfo()
{
	// Override for child class
}

void AMageCharacterBase::AddCharacterAbilites()
{
	if (!HasAuthority()) return; // 只能技能只能在服务器被赋予

	UMageAbilitySystemComponent* MageASC = CastChecked<UMageAbilitySystemComponent>(GetAbilitySystemComponent());
	MageASC->AddCharacterAbilites(StartupAbilites);
}

void AMageCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect>& InitAttributeEffects, float Level)
{
	check(IsValid(GetAbilitySystemComponent()));
	check(InitAttributeEffects);

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(
		InitAttributeEffects, 1.f, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AMageCharacterBase::InitDefaultAttributes()
{
	ApplyEffectToSelf(DefaultPrimaryEffects, 1.f);
	ApplyEffectToSelf(DefaultSecondaryEffects, 1.f);
	ApplyEffectToSelf(DefaultBaseEffects, 1.f);
}