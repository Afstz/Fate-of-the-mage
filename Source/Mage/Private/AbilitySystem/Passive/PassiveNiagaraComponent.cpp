// Copyright AFstz.


#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "MageGameplayTags.h"
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
		ActiveIfEquipped(MageASC);
	}
	else if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner())) // ASC未初始化成功
	{
		CombatInterface->GetASCRegisteredDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* InASC)
		{
			if (UMageAbilitySystemComponent* MageASC = Cast<UMageAbilitySystemComponent>(InASC))
			{
				MageASC->OnPassiveActivationChanged.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
				ActiveIfEquipped(MageASC);
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

void UPassiveNiagaraComponent::ActiveIfEquipped(UMageAbilitySystemComponent* MageASC)
{
	// 初始化完毕并且被动技能是装备状态就激活特效。 
	if (MageASC->bStartupAbilitiesGiven) 
	{
		const FGameplayTag StatusTag = MageASC->GetStatusTagFromAbilityTag(PassiveAbilityTag);

		if (StatusTag.MatchesTagExact(FMageGameplayTags::Get().Abilities_Status_Equipped))
		{
			Activate();
		}
	}
}