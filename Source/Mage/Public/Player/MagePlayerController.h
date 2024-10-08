// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MagePlayerController.generated.h"

class UMageUserWidget;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;

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
	virtual void PlayerTick(float DeltaTime) override;
	
	/** Attribute Menu */
	UPROPERTY(BlueprintReadWrite)
	bool IsOpenAttributeMenu = false;
private:
	/** Input */
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> MageInputContext;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	void Move(const FInputActionValue& InputAction);
	
	/** Attribute Menu */
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> AttributeMenuAction;
	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	TSubclassOf<UMageUserWidget> AttributeMenuWidgetClass;
	UPROPERTY()
	TObjectPtr<UMageUserWidget> AttributeMenuWidget;
	void AttributeMenu(const FInputActionValue& InputAction);
	
	void CursorTrace();
	IEnemyInterface* LastActor;
	IEnemyInterface* CurrentActor;
};
