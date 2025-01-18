// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbility/ProjectileSpellAbility.h"
#include "FireBallAbility.generated.h"

/**
 * 
 */
UCLASS()
class MAGE_API UFireBallAbility : public UProjectileSpellAbility
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 AbilityLevel) override;
	virtual FString GetNextLevelDescription(int32 AbilityLevel) override;
	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(const FVector& ProjectileLocation, const bool bOverridePitch, float PitchOverride, AActor* HomingTarget);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "FireBall")
	int32 MaxNumProjectiles = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "FireBall")
	float SpawnSpread = 90.f; // 火球生成范围
	
	UPROPERTY(EditDefaultsOnly, Category = "FireBall")
	float MinHomingAcceleration = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "FireBall")
	float MaxHomingAcceleration = 1100.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "FireBall")
	bool bHomingTarget = true;
};
