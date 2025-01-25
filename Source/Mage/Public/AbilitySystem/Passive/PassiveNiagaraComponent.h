// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "PassiveNiagaraComponent.generated.h"

/**
 *  被动技能粒子组件
 */
UCLASS()
class MAGE_API UPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
public:
	UPassiveNiagaraComponent();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveAbilityTag;
	
	void OnPassiveActivate(const FGameplayTag& PassiveTag, const bool bActivate);
};
