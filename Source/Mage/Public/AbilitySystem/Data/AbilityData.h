// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityData.generated.h"


USTRUCT(BlueprintType)
struct FMageAbilityData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTag AbilityTag = FGameplayTag(); // 技能标签

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag AbilityInputTag = FGameplayTag(); // 技能输入标签
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FGameplayTag AbilityCooldownTag = FGameplayTag(); // 技能冷却标签

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<const UTexture2D> IconTexture = nullptr; // 技能图标

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<const UMaterialInterface> BackgroundImage = nullptr; // 技能背景
};

/**
 *  存储技能信息相关配置
 */
UCLASS()
class MAGE_API UAbilityData : public UDataAsset
{
	GENERATED_BODY()
public:
	FMageAbilityData FindAbilityDataForTag(const FGameplayTag& AbilityTag, bool bWarnIfNotFound = true);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FMageAbilityData> AbilitiesData;
};
