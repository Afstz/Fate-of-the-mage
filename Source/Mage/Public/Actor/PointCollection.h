// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointCollection.generated.h"

UCLASS()
class MAGE_API APointCollection : public AActor
{
	GENERATED_BODY()

public:
	APointCollection();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, BlueprintCallable)
	TArray<USceneComponent*> GetGroundPoints(const FVector& PointCenter, int32 NumPoints, float YawOverride); // 得到指定数量有效的点
protected:
	virtual void BeginPlay() override;

	// 所有可用的生成点
	UPROPERTY()
	TArray<USceneComponent*> ImmutablePoints; 

	// 中心点
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Pt_Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Pt_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Pt_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Pt_3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Pt_4;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Pt_5;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Pt_6;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Pt_7;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Pt_8;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Pt_9;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Pt_10;
	
};