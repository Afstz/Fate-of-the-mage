// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "DamageGameplayAbility.h"
#include "BeamSpellAbility.generated.h"

/**
 * 
 */
UCLASS()
class MAGE_API UBeamSpellAbility : public UDamageGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Beam")
	void StoreOwnerVariables();
	UFUNCTION(BlueprintCallable, Category = "Beam")
	void StoreMouseHitResult(const FHitResult& MouseHit);
	UFUNCTION(BlueprintCallable, Category = "Beam")
	void TraceFirstTarget(const FVector& BeamEndLocation); // 追踪射线的第一个目标
	UFUNCTION(BlueprintCallable, Category = "Beam")
	void StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets);

	UFUNCTION(BlueprintImplementableEvent, Category = "Beam")
	void FirstTargetDied(AActor* FirstTarget);
	UFUNCTION(BlueprintImplementableEvent, Category = "Beam")
	void AdditionalTargetDied(AActor* AdditionalTarget);
protected:
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	FVector MouseHitLocation = FVector::Zero();
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<AActor> MouseHitActor;
	
	UPROPERTY(BlueprintReadOnly, Category = "Beam")
	TObjectPtr<APlayerController> OwnerController;
	UPROPERTY(BlueprintReadOnly, Category = "Beam")
	TObjectPtr<ACharacter> OwnerCharacter;
	UPROPERTY(EditDefaultsOnly, Category = "Beam")
	int32 NumMaxShockTargets = 4.f;
};
