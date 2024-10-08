 // Copyright AFstz.


#include "Character/MageCharacterBase.h"
#include "AbilitySystemComponent.h"

 AMageCharacterBase::AMageCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

 UAbilitySystemComponent* AMageCharacterBase::GetAbilitySystemComponent() const
 {
	return AbilitySystemComponent;
 }

 void AMageCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

 void AMageCharacterBase::InitAbilityActorInfo()
 {
 }

 void AMageCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect>& InitAttributeEffects, float Level)
 {
 	check(IsValid(GetAbilitySystemComponent()));
 	check(InitAttributeEffects);
 	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
 	ContextHandle.AddSourceObject(this);
 	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(InitAttributeEffects, 1.f, ContextHandle);
 	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
 }

 void AMageCharacterBase::InitDefaultAttributes()
 {
 	ApplyEffectToSelf(DefaultPrimaryEffects, 1.f);
 	ApplyEffectToSelf(DefaultSecondaryEffects, 1.f);
 	ApplyEffectToSelf(DefaultBaseEffects, 1.f);
 }




