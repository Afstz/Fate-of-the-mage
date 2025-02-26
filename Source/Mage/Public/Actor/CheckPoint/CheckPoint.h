// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interface/SaveInterface.h"
#include "CheckPoint.generated.h"

class USphereComponent;
/**
 *  游戏地图内存档点, PlayerStartTag设置
 */
UCLASS()
class MAGE_API ACheckPoint : public APlayerStart, public ISaveInterface
{
	GENERATED_BODY()
public:
	ACheckPoint(const FObjectInitializer& ObjectInitializer);

	/** Save Interface */
	virtual bool ShouldLoadTransform_Implementation() override { return false; }
	virtual void LoadActor_Implementation() override;
	
	// 需要被序列化的字段
	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bReached = false;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void HandleCheckPointGlow();
	UFUNCTION(BlueprintImplementableEvent)
	void CheckPointGlow(UMaterialInstanceDynamic* DynamicMaterialInstance);
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CheckPointMesh;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
	
};
