// Copyright AFstz.


#include "Player/MagePlayerState.h"
#include "AbilitySystem/MageAbilitySystemComponent.h"
#include "AbilitySystem/MageAttributeSet.h"
#include "Net/UnrealNetwork.h"

AMagePlayerState::AMagePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UMageAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UMageAttributeSet>(TEXT("AttributeSet"));
	
	NetUpdateFrequency = 100.f;
}

void AMagePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMagePlayerState, Level);
	DOREPLIFETIME(AMagePlayerState, XP);
	DOREPLIFETIME(AMagePlayerState, AttributePoints);
	DOREPLIFETIME(AMagePlayerState, SkillPoints);
}

UAbilitySystemComponent* AMagePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMagePlayerState::SetLevel(const int32 InLevel)
{
	Level = InLevel;
	LevelChangedDelegate.Broadcast(Level);
}

void AMagePlayerState::AddToLevel(const int32 InLevel)
{
	Level += InLevel;
	LevelChangedDelegate.Broadcast(Level);
}

void AMagePlayerState::SetXP(const int32 InXP)
{
	XP = InXP;
	XPChangedDelegate.Broadcast(XP);
}

void AMagePlayerState::AddToXP(const int32 InXP)
{
	XP += InXP;
	XPChangedDelegate.Broadcast(XP);
}

void AMagePlayerState::AddToAttributePoints(const int32 InAttributePoints)
{
	AttributePoints += InAttributePoints;
	AttributePointChangedDelegate.Broadcast(AttributePoints);
}

void AMagePlayerState::AddToSkillPoints(const int32 InSkillPoints)
{
	SkillPoints += InSkillPoints;
	SkillPointChangedDelegate.Broadcast(SkillPoints);
}

void AMagePlayerState::OnRep_Level(int32 OldLevel)
{
	// 客户端同步
	LevelChangedDelegate.Broadcast(Level);
}

void AMagePlayerState::Onrep_XP(int32 OldXP)
{
	XPChangedDelegate.Broadcast(XP);
}

void AMagePlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	AttributePointChangedDelegate.Broadcast(AttributePoints);
}

void AMagePlayerState::OnRep_SkillPoints(int32 OldSkillPoints)
{
	SkillPointChangedDelegate.Broadcast(SkillPoints);
}