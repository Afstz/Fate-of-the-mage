// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "MageProjectile.h"
#include "FireBlast.generated.h"

UCLASS()
class MAGE_API AFireBlast : public AMageProjectile
{
	GENERATED_BODY()

public:
	AFireBlast();

	// 执行火球开始向外发射时间轴事件
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void StartFireBlastTimeline();
protected:
	virtual void BeginPlay() override;
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	virtual void PlayImpactEffect() override;
private:
};