// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

USTRUCT(BlueprintType)
struct FTaggedMontage // 蒙太奇与对应的事件标签和武器插槽数据
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> CombatMontage = nullptr; // 蒙太奇动画
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageEventTag; // 蒙太奇事件标签
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CombatSocketTag; // 战斗攻击方式标签
	
	UPROPERTY(EditDefaultsOnly)
	FName CombatSocketName; // 触发伤害的骨骼插槽

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundBase> ImpactSound; // 击中音效
};

// This class does not need to be modified.
UINTERFACE()
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MAGE_API IEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetFacingTarget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetFacingTarget(AActor* InFacingTarget);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetSocketLocationByStruct(const FTaggedMontage& TaggedMontage) const; // 通过结构体获取插槽位置
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<FTaggedMontage> GetTaggedMontage() const; // 获取设置的标记的蒙太奇结构体数组
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);
};