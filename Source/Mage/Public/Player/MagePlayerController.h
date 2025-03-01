// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "MagePlayerController.generated.h"

class IHighlightInterface;
class AMagicCircle;
class UNiagaraSystem;
class UDamageTextComponent;
class USplineComponent;
class UMageAbilitySystemComponent;
class UMageInputData;
class UMageUserWidget;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;


enum class ETargetingStatus : uint8
{
	TargetingEnemy,
	TargetingNoneEnemy,
	NoneTarget
};

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
	
	/** Magic Circle Function */
	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);
	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;
	
	/** Menu */
	UFUNCTION(BlueprintPure)
	FORCEINLINE UMageUserWidget* GetAttributeMenuWidget() { return AttributeMenuWidget; }
	UFUNCTION(BlueprintCallable)
	void SetAttributeMenuWidget(UMageUserWidget* InAttributeMenu);
	UFUNCTION(BlueprintPure)
	FORCEINLINE UMageUserWidget* GetSkillMenuWidget() { return SkillMenuWidget; }
	UFUNCTION(BlueprintCallable)
	void SetSkillMenuWidget(UMageUserWidget* InSkillMenu);

	/** DamageFloatingText */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextWidgetCompClass;
private:
	/** Input */
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> MageInputContext;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	void Move(const FInputActionValue& InputActionValue);
	
	/** Attribute Menu */
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> AttributeMenuAction;
	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	TSubclassOf<UMageUserWidget> AttributeMenuWidgetClass;
	UPROPERTY()
	TObjectPtr<UMageUserWidget> AttributeMenuWidget;
	void AttributeMenu(const FInputActionValue& InputActionValues);
	/** Skill Menu*/
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> SkillMenuAction;
	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	TSubclassOf<UMageUserWidget> SkillMenuWidgetClass;
	UPROPERTY()
	TObjectPtr<UMageUserWidget> SkillMenuWidget;
	void SkillMenu(const FInputActionValue& InputActionValue);
	
	/** Shift Key Attack*/
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ShiftAction;
	FORCEINLINE void ShiftKeyPressed() { bShiftKeyPressed = true;}
	FORCEINLINE void ShiftKeyReleased() { bShiftKeyPressed = false;}
	bool bShiftKeyPressed = false;

	/** Show Outline */
	void CursorTrace();
	UPROPERTY()
	TObjectPtr<AActor> LastCursorActor;
	UPROPERTY()
	TObjectPtr<AActor> CurrentCursorActor;
	FHitResult CursorHit;
	ETargetingStatus TargetingStatus = ETargetingStatus::NoneTarget;
	void HighlightActor(AActor* InActor);
	void UnHighlightActor(AActor* InActor);
	

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
	bool bAutoRunning = false; // 自动跑步
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 30.f; // 停止跑步的范围
	UPROPERTY()
	TObjectPtr<USplineComponent> Spline;
	void AutoRunning();
	UPROPERTY(EditDefaultsOnly, Category = "Run")
	TObjectPtr<UNiagaraSystem> MouseClickEffect; // 鼠标单击地板特效
	
	/** Magic Circle*/
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;
	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircleDecal; // 魔法贴花
	void UpdateMagicCircleLocation() const;

	bool IsCharacterDead();
};
