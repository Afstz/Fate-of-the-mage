// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "Actor/CheckPoint/CheckPoint.h"
#include "MapEntrance.generated.h"

/**
 * 
 */
UCLASS()
class MAGE_API AMapEntrance : public ACheckPoint
{
	GENERATED_BODY()
public:
	AMapEntrance(const FObjectInitializer& ObjectInitializer);

	/** Save Interface */
	virtual void LoadActor_Implementation() override;
	/** Highlight Interface */
	virtual void HighlightActor_Implementation() override;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationMap;
	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStart;
protected:
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	
};
