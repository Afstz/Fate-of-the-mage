// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeData.generated.h"

USTRUCT(BlueprintType)
struct FMageAttributeData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescriptaion = FText();

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;
};

/**
 * 存储要查找的属性信息 用于查找然后暴露给蓝图UI层
 */
UCLASS()
class MAGE_API UAttributeData : public UDataAsset
{
	GENERATED_BODY()
public:
	FMageAttributeData FindAttributeDataForTag(const FGameplayTag& AttributeTag) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FMageAttributeData> AttributesData;
	
};
