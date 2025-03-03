// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "MageEffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;
class USphereComponent;

/**
 *  应用与移除策略枚举
 *  把C++暴露给蓝图使程序变得可拓展且灵活
 */

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnBeginOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};

UCLASS()
class MAGE_API AMageEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMageEffectActor();
	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(AActor* TargetActor); // 开始重叠时执行自己设计的逻辑
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor); // 离开重叠时执行自己设计的逻辑

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Effects")
	bool bApplyEffectToEnemy = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Effects")
	bool bDestroyOnEffectApplication = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> InstantEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> DurationEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> InfiniteEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Effects")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::DoNotRemove;

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles; // 存放已有的无限时间效果

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn), Category = "Gameplay Effects")
	float ActorCurveLevel = 1.f;

	/** Actor Effect */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Effects")
	bool bRotate = false;
	UPROPERTY(BlueprintReadWrite, Category = "Spawn Effects")
	FRotator CalculatedRotation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Effects")
	float RotateRate = 45.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Effects")
	bool bSinusoidal = false;
	float SineTime = 0.f;
	FVector OriginLocation;
	UPROPERTY(BlueprintReadWrite, Category = "Spawn Effects")
	FVector CalculatedLocation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Effects")
	float SineAmplitude = 8.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Effects")
	float SineFrequency = 4.f;

	UFUNCTION(BlueprintCallable, Category = "Spawn Effects")
	void StartMovement();
	void ActorMovement(float DeltaSeconds);
};
