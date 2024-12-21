// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/WidgetController/MageWidgetController.h"
#include "MageHUD.generated.h"

class USkillMenuWidgetController;
class UAttributeMenuWidgetController;
struct FWidgetControllerParams;
struct FWigetControllerParams;
class UAttributeSet;
class UAbilitySystemComponent;
class UOverlayWidgetController;
class UMageUserWidget;
/**
 * 
 */
UCLASS()
class MAGE_API AMageHUD : public AHUD
{
	GENERATED_BODY()

public:
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& InWidgetControllerParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& InWidgetControllerParams);
	USkillMenuWidgetController* GetSkillMenuWidgetController(const FWidgetControllerParams& InWidgetControllerParams);
	
	UFUNCTION(BlueprintCallable)
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
protected:
	
private:
	/** OverlayWidget */
	UPROPERTY()
	TObjectPtr<UMageUserWidget> OverlayWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMageUserWidget> OverlayWidgetClass;

	/** OverlayWidgetController */
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	
	/** AttributeMenuWidgetController */
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	/** SkillMenuWidgetController */
	UPROPERTY()
	TObjectPtr<USkillMenuWidgetController> SkillMenuWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<USkillMenuWidgetController> SkillMenuWidgetControllerClass;
};
