// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "MageProjectile.generated.h"

class UNiagaraSystem;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class MAGE_API AMageProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AMageProjectile();
	virtual void Destroyed() override;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle EffectSpecHandle;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float EnableCollisionDelay = 0.f;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Projectile")
	void DelayEnableCollision(); // 延迟启用碰撞
	
	void PlayImpactEffect();
	
	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TObjectPtr<UNiagaraSystem> ImpactNiagara;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TObjectPtr<USoundBase> ImpactSound;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TObjectPtr<USoundBase> FlightingSound;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float LifeSpan = 10.f;
};
