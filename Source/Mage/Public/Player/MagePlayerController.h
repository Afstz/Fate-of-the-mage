// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "MagePlayerController.generated.h"

class UDamageTextComponent;
class USplineComponent;
class UMageAbilitySystemComponent;
class UMageInputData;
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

	/** DamageFloatingText */
	UFUNCTION(Client, Reliable)
	void ShowDamageText(ACharacter* TargetCharacter, float DamageValue, const bool bIsCriticalHit, const bool bIsBlockHit);
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;
	
	/** Attribute Menu */
	UPROPERTY(BlueprintReadWrite)
	bool IsOpenAttributeMenu = false;

	/** DamageFloatingText */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextWidgetCompClass;
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
	/** Shift Key Attack*/
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ShiftAction;
	FORCEINLINE void ShiftKeyPressed() { bShiftKeyPressed = true;}
	FORCEINLINE void ShiftKeyReleased() { bShiftKeyPressed = false;}
	bool bShiftKeyPressed = false;

	/** Show Enemy Outline*/
	void CursorTrace();
	IEnemyInterface* LastCursorActor;
	IEnemyInterface* CurrentCursorActor;
	FHitResult CursorHit;

	/** Ability */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UMageInputData> MageInputData;
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);

	/** Ability System Component*/
	UPROPERTY()
	TObjectPtr<UMageAbilitySystemComponent> MageASC;
	UMageAbilitySystemComponent* GetMageAbilitySystemComponent();

	/** Auto Running */
	FVector CachedDestination = FVector::ZeroVector; // 导航目的地
	float HoldingTime = 0.f; // 按住时间
	float ShortPressThreshold = 0.25f; // 短按阈值
	bool bTargeting = false; // 鼠标是否碰到敌人
	bool bAutoRunning = false; // 自动跑步
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 30.f; // 停止跑步的范围
	UPROPERTY()
	TObjectPtr<USplineComponent> Spline;
	void AutoRunning();
};
