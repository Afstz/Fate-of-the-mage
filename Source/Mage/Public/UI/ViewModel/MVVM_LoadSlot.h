// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableLoadArchiveButton, bool, bEnable);

UENUM(BlueprintType)
enum ELoadSlotStatus : uint8
{
	Vacant,
	EnterName,
	Taken
};

/**
 *  
 */
UCLASS()
class MAGE_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	// 初始化插槽状态
	void InitializeSlot();

	void SetLoadSlotName(const FString& InLoadSlotName);
	void SetCharacterName(const FString& InCharacterName);
	void SetPlayerLevel(const int32 InPlayerLevel);
	void SetMapName(const FString& InMapName);
	
	FString GetLoadSlotName() const { return LoadSlotName; }
	FString GetCharacterName() const { return CharacterName; }
	int32 GetPlayerLevel() const { return PlayerLevel; }
	FString GetMapName() const { return MapName; }

	/* Delegate */
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;
	UPROPERTY(BlueprintAssignable)
	FEnableLoadArchiveButton EnableLoadArchiveButton;
	
	// 游戏存档插槽的Index
	UPROPERTY()
	int32 SlotIndex;
	
	// 起始点标签
	UPROPERTY()
	FName PlayerStartTag;

	// 当前插槽的状态,TEnumAsByte用于检测类型不匹配的错误
	UPROPERTY()
	TEnumAsByte<ELoadSlotStatus> LoadSlotStatus;
private:
	// 游戏存档插槽名
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	FString LoadSlotName = FString();
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	FString CharacterName = FString();

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	int32 PlayerLevel = 1;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess))
	FString MapName = FString();
};
