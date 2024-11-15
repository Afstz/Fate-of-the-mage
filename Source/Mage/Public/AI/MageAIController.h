// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MageAIController.generated.h"

class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class MAGE_API AMageAIController : public AAIController
{
	GENERATED_BODY()
public:
	AMageAIController();
private:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
