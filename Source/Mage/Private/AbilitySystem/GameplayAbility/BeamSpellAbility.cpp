// Copyright AFstz.


#include "AbilitySystem/GameplayAbility/BeamSpellAbility.h"

#include "AbilitySystem/MageAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interface/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UBeamSpellAbility::StoreOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

void UBeamSpellAbility::StoreMouseHitResult(const FHitResult& MouseHit)
{
	if (MouseHit.bBlockingHit)
	{
		MouseHitLocation = MouseHit.ImpactPoint;
		MouseHitActor = MouseHit.GetActor();
	}
}

void UBeamSpellAbility::TraceFirstTarget(const FVector& BeamEndLocation)
{
	if (!OwnerCharacter) return;

	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		if (USkeletalMeshComponent* WeaponMesh = ICombatInterface::Execute_GetWeaponMesh(OwnerCharacter))
		{
			FHitResult FirstHit;
			TArray<AActor*> ActorsToIgnore;
			FVector SocketLocation = WeaponMesh->GetSocketLocation("TipSocket");
			UKismetSystemLibrary::SphereTraceSingle(
				OwnerCharacter,
				SocketLocation,
				BeamEndLocation,
				10.f,
				TraceTypeQuery1,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::None,
				FirstHit,
				true
			);
			if (FirstHit.bBlockingHit) // 防止射线穿过敌人
			{
				MouseHitLocation = FirstHit.ImpactPoint;
				MouseHitActor = FirstHit.GetActor();
			}
		}
	}
	
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(MouseHitActor)) // 绑定角色死亡时委托
	{
		// 判断是否已经绑定委托
		if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UBeamSpellAbility::FirstTargetDied))
		{
			CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UBeamSpellAbility::FirstTargetDied);
		}
	}
}

void UBeamSpellAbility::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(MouseHitActor);

	TArray<AActor*> OverlapActors;
	UMageAbilitySystemLibrary::GetAlivePlayerInSphereRadius( // 得到范围内所有角色
		GetAvatarActorFromActorInfo(),
		OverlapActors,
		ActorsToIgnore,
		400.f,
		MouseHitLocation
	);
	
	int32 MaxAdditionalTarget = FMath::Min(NumMaxShockTargets, GetAbilityLevel()); // 最大链接数量
	
	UMageAbilitySystemLibrary::GetClosetActors(MaxAdditionalTarget, OverlapActors, OutAdditionalTargets, MouseHitActor);

	for (auto It = OutAdditionalTargets.CreateIterator(); It; ++It)
	{
		if (UMageAbilitySystemLibrary::IsNotFriend(*It, MouseHitActor)) // 不是队友移除Actor
		{
			It.RemoveCurrent();
		}
	}

	for (AActor* AdditionalTarget : OutAdditionalTargets) // 绑定角色死亡时委托
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(AdditionalTarget))
		{
			if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UBeamSpellAbility::AdditionalTargetDied))
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UBeamSpellAbility::AdditionalTargetDied);
			}
		}
	}
}

void UBeamSpellAbility::ClearDelegate(AActor* TargetActor)
{
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetActor))
	{
		if (CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UBeamSpellAbility::FirstTargetDied))
		{
			CombatInterface->GetOnDeathDelegate().RemoveDynamic(this, &UBeamSpellAbility::FirstTargetDied);
		}
		if (CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UBeamSpellAbility::AdditionalTargetDied))
		{
			CombatInterface->GetOnDeathDelegate().RemoveDynamic(this, &UBeamSpellAbility::AdditionalTargetDied);
		}
	}
}