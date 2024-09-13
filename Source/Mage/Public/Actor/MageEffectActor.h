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
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(AActor* TargetActor); // 开始重叠时执行自己设计的逻辑
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor); // 离开重叠时执行自己设计的逻辑

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Effects")
	float ActorCurveLevel = 1.f;
};
