// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "MageInputData.generated.h"


class UInputAction;

USTRUCT(BlueprintType)
struct FMageInputActionData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputGameplayTag = FGameplayTag();
};

/**
 *  存储技能动作映射与按键标签信息
 */
UCLASS()
class MAGE_API UMageInputData : public UDataAsset
{
	GENERATED_BODY()
	
public:

	const UInputAction* FindInputActionForTag(const FGameplayTag& FindInputTag, bool bWarnIfNotFound = true);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FMageInputActionData> AbilityInputActions;
};
