// Copyright AFstz.


#include "Actor/MageEffectActor.h"
#include "ActiveGameplayEffectHandle.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AMageEffectActor::AMageEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("SenceComp")));
}

void AMageEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AMageEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> EffectDefaultClass)
{
	if (!bApplyEffectToEnemy && TargetActor->ActorHasTag(FName("Enemy"))) return;
		
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;
	check(EffectDefaultClass);
	
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext(); // 获取上下文句柄
	EffectContextHandle.AddSourceObject(this); // 设置句柄的创建者(上下文信息)
	// Effect实例化的句柄
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(EffectDefaultClass, ActorCurveLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	
	// 判断持续时间是否是无限
	bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	// 在是无限时间效果和需要在结束时清除掉时，将效果句柄添加到Map
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
	
	if (bDestroyOnEffectApplication) // 不是永久效果就摧毁
	{
		Destroy();
	}
}

void AMageEffectActor::OnBeginOverlap(AActor* TargetActor)
{
	if (!bApplyEffectToEnemy && TargetActor->ActorHasTag(FName("Enemy"))) return;
	
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteEffectClass);
	}
}

void AMageEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (!bApplyEffectToEnemy && TargetActor->ActorHasTag(FName("Enemy"))) return;
	
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteEffectClass);
	}

	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) // 删除永久持续时间效果
	{
		UAbilitySystemComponent* TargetActorASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetActorASC)) return;

		for (auto It = ActiveEffectHandles.CreateIterator(); It; ++It)
		{
			if (TargetActorASC == It.Value())
			{
				TargetActorASC->RemoveActiveGameplayEffect(It.Key(), 1);
				It.RemoveCurrent();
			}
		}

		// 第二种方法遍历
		
		/*TArray<FActiveGameplayEffectHandle> ActiveEffectArray; // 临时存放要删除的ActiveEffectHandles
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*>& HandlePair : ActiveEffectHandles)
		{
			if (TargetActorASC == HandlePair.Value) // 删除对应玩家的效果
			{
				TargetActorASC->RemoveActiveGameplayEffect(HandlePair.Key, 1); // 从对应ASC效果堆栈上删除一个效果
				ActiveEffectArray.Add(HandlePair.Key);
			}
		}
		for (FActiveGameplayEffectHandle& ActiveEffect : ActiveEffectArray)
		{
			ActiveEffectHandles.Remove(ActiveEffect);
		}*/
	}
 }

