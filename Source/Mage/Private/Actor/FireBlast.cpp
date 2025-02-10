// Copyright AFstz.


#include "Actor/FireBlast.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayCueManager.h"
#include "MageGameplayTags.h"
#include "AbilitySystem/MageAbilitySystemLibrary.h"
#include "Components/SphereComponent.h"

AFireBlast::AFireBlast()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFireBlast::BeginPlay()
{
	Super::BeginPlay();
}

void AFireBlast::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;
	
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;
			
			UMageAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
	}
}

void AFireBlast::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void AFireBlast::PlayImpactEffect()
{
	if (GetOwner()) // 防止服务器没有复制,客户端会访问到空指针
	{
		FGameplayCueParameters CueParameters;
		CueParameters.Location = GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), FMageGameplayTags::Get().GameplayCue_FireBlast, CueParameters);
	}
}

