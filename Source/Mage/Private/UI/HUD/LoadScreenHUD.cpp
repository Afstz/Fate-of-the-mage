// Copyright AFstz.


#include "UI/HUD/LoadScreenHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/ViewModel/MVVM_LoadScreen.h"
#include "UI/Widget/LoadScreenWidget.h"

void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();

	LoadScreenViewModel = NewObject<UMVVM_LoadScreen>(this, LoadScreenViewModelClass);
	LoadScreenViewModel->SetLoadScreenName(FString("LoadScreenWidgetVM"));
	LoadScreenViewModel->InitializeLoadSlotViewModels(); // 初始化LoadSlotViewModels
	
	LoadScreenWidget = CreateWidget<ULoadScreenWidget>(GetWorld(), LoadScreenWidgetClass);
	LoadScreenWidget->AddToViewport();
	LoadScreenWidget->BlueprintInitializeWidget(); // 初始化蓝图Widget与LoadSlotViewModel相关联

	LoadScreenViewModel->LoadSaveData(); // 加载存档

	FInputModeUIOnly InputModeUIOnly;
	InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	GetOwningPlayerController()->SetInputMode(InputModeUIOnly);
	GetOwningPlayerController()->SetShowMouseCursor(true);
}