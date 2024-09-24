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

UAbilitySystemComponent* AMagePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMagePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMagePlayerState, Level);
}

void AMagePlayerState::OnRep_Level(int32 OldLevel)
{
	
}