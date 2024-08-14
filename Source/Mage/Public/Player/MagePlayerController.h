// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MagePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
/**
 * 
 */
UCLASS()
class MAGE_API AMagePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMagePlayerController();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> MageInputContext;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	void Move(const FInputActionValue& InputAction);
	
};
