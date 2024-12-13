// Copyright AFstz.


#include "Character/MageCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/MageAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
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

FVector AMageCharacterBase::GetLocationByWeaponSocket_Implementation() const
{
	check(Weapon);
	// 如果没有武器就在身上找
	if (!Weapon->GetSkeletalMeshAsset())
	{
		return GetMesh()->GetSocketLocation(WeaponTipSocketName);
	}
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

UAnimMontage* AMageCharacterBase::GetHitReactMontage_Implementation() const
{
	return HitReactMontage;
}

void AMageCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MultiHandleDeath();
}

bool AMageCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AMageCharacterBase::GetAvatarActor_Implementation()
{
	return this;
}

UNiagaraSystem* AMageCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

int32 AMageCharacterBase::GetMinionCount_Implementation() const
{
	return MinionCount;
}

void AMageCharacterBase::AddMinionCount_Implementation(const int32 InMinionCount)
{
	MinionCount += InMinionCount;
}

ECharacterClass AMageCharacterBase::GetCharacterClass_Implementation() const
{
	return CharacterClass;
}

void AMageCharacterBase::MultiHandleDeath_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	
	Weapon->SetSimulatePhysics(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Weapon->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
	
	bDead = true;
}

void AMageCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMageCharacterBase::InitAbilityActorInfo()
{
	// Override for child class
}

void AMageCharacterBase::Dissolve()
{
	TArray<UMaterialInstanceDynamic*> MaterialInstDynamics;
	if (CharacterMaterialInst)
	{
		UMaterialInstanceDynamic* MaterialInstDynamic = UMaterialInstanceDynamic::Create(CharacterMaterialInst, this);
		GetMesh()->SetMaterial(0, MaterialInstDynamic);
		MaterialInstDynamics.Add(MaterialInstDynamic);
	}
	if (WeaponMaterialInst)
	{
		UMaterialInstanceDynamic* MaterialInstDynamic = UMaterialInstanceDynamic::Create(WeaponMaterialInst, this);
		Weapon->SetMaterial(0, MaterialInstDynamic);
		MaterialInstDynamics.Add(MaterialInstDynamic);
	}
	StartDissolveTimeLine(MaterialInstDynamics);
}

void AMageCharacterBase::AddCharacterAbilites()
{
	if (!HasAuthority()) return; // 只能技能只能在服务器被赋予

	UMageAbilitySystemComponent* MageASC = CastChecked<UMageAbilitySystemComponent>(GetAbilitySystemComponent());
	MageASC->AddCharacterAbilites(StartupAbilites); // 主动技能
	MageASC->AddCharacterPassiveAbilites(StartupPassiveAbilites); // 被动技能
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