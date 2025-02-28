// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interface/HighlightInterface.h"
#include "Interface/SaveInterface.h"
#include "CheckPoint.generated.h"

class USphereComponent;
/**
 *  游戏地图内存档点, PlayerStartTag设置
 */
UCLASS()
class MAGE_API ACheckPoint : public APlayerStart, public ISaveInterface, public IHighlightInterface
{
	GENERATED_BODY()
public:
	ACheckPoint(const FObjectInitializer& ObjectInitializer);

	/** Save Interface */
	virtual bool ShouldLoadTransform_Implementation() override { return false; }
	virtual void LoadActor_Implementation() override;
	/** Highlight Interface */
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void MoveToLocation_Implementation(FVector& OutDestination) override;
	
	// 需要被序列化的字段
	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bReached = false;
	
	// Red:250 Blue:251 TAN:252
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 CustomDepthStencilValue = 251;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void HandleCheckPointGlow();
	UFUNCTION(BlueprintImplementableEvent)
	void CheckPointGlow(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CheckPointMesh;
	// 点击移动的目的地
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DestinationComponent;
private:
	
};
