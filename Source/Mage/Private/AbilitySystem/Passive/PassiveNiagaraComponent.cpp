// Copyright AFstz.


#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/MageAbilitySystemComponent.h"
#include "Interface/CombatInterface.h"

UPassiveNiagaraComponent::UPassiveNiagaraComponent()
{
	bAutoActivate = false;
}

void UPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	
	UMageAbilitySystemComponent* MageASC = Cast<UMageAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()));
	if (MageASC)
	{
		MageASC->OnPassiveActivationChanged.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
	}
	else if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner())) // ASC未初始化成功
	{
		CombatInterface->GetASCRegisteredDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* InASC)
		{
			if (UMageAbilitySystemComponent* MageASC = Cast<UMageAbilitySystemComponent>(InASC))
			{
				MageASC->OnPassiveActivationChanged.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
			}
		});
	}
}

void UPassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& PassiveTag, const bool bInActivate)
{
	if (PassiveAbilityTag.MatchesTagExact(PassiveTag))
	{
		if (bInActivate)
		{
			if (!IsActive()) Activate(); // 不重复激活
		}
		else
		{
			Deactivate();
		}
	}
}