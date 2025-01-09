// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "DebuffNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class MAGE_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
public:
	UDebuffNiagaraComponent();

	UPROPERTY(VisibleAnywhere)
	FGameplayTag DebuffTag;
	
protected:
	virtual void BeginPlay() override;

	void OnDebuffTagChanged(const FGameplayTag InDebuffTag, int32 NewCount);
	UFUNCTION()
	void OnOwnerDead(AActor* DeadActor);

private:
	
};

