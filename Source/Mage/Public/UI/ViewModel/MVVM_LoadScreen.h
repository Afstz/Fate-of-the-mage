// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

class UMVVM_LoadSlot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLoadArchiveSelected);
/**
 *  主ViewModel管理着三个LoadSlotViewModel
 */
UCLASS()
class MAGE_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

	// 初始化LoadSlotViewModel,由LoadScreen来管理
	void InitializeLoadSlotViewModels();

	void LoadSaveData();

	UFUNCTION(BlueprintPure, BlueprintCallable)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index) const;

	/** Delegate Evenet */
	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 InSlotIndex);
	UFUNCTION(BlueprintCallable)
	void CreateCharacterButtonPressed(int32 InSlotIndex, const FString& CharacterName);
	UFUNCTION(BlueprintCallable)
	void LoadArchiveButtonPressed(int32 InSlotIndex);
	UFUNCTION(BlueprintCallable)
	void StartGameButtonPressed();
	// 删除存档警告对话框的确认按钮按下时执行
	UFUNCTION(BlueprintCallable)
	void DialogConfirmButtonPressed();

	// Test
	void SetLoadScreenName(const FString& InLoadScreenName);

	UPROPERTY(BlueprintAssignable)
	FLoadArchiveSelected LoadArchiveSelected;
private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, meta = (AllowPrivateAccess))
	FString LoadScreenName = FString();
	
	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlotViewModels;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlotViewModel_0;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlotViewModel_1;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlotViewModel_2;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> SelectedSlotVM;
};
