// Copyright AFstz.


#include "AbilitySystem/MageAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "MageAbilityTypes.h"
#include "MageGameplayTags.h"
#include "Engine/OverlapResult.h"
#include "Game/MageGameModeBase.h"
#include "Interface/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MagePlayerState.h"
#include "UI/HUD/MageHUD.h"
#include "UI/WidgetController/MageWidgetController.h"

bool UMageAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AMageHUD*& OutMageHUD)
{
	// 根据提供的世界上下文信息找到当前世界的全部PlayerController, 0 为本地控制的角色
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutMageHUD = Cast<AMageHUD>(PC->GetHUD());
		if (OutMageHUD)
		{
			AMagePlayerState* PS = PC->GetPlayerState<AMagePlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			OutWCParams.PlayerController = PC;
			OutWCParams.PlayerState = PS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.AttributeSet = AS;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UMageAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WidgetControllerParams;
	AMageHUD* MageHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WidgetControllerParams, MageHUD))
	{
		return MageHUD->GetOverlayWidgetController(WidgetControllerParams);
	}
	return nullptr; 
}

UAttributeMenuWidgetController* UMageAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WidgetControllerParams;
	AMageHUD* MageHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WidgetControllerParams, MageHUD))
	{
		return MageHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
	}
	return nullptr; 
}

USkillMenuWidgetController* UMageAbilitySystemLibrary::GetSkillMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WidgetControllerParams;
	AMageHUD* MageHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WidgetControllerParams, MageHUD))
	{
		return MageHUD->GetSkillMenuWidgetController(WidgetControllerParams);
	}
	return nullptr; 
}

void UMageAbilitySystemLibrary::InitCharacterDefaultAttributes(const UObject* WorldContextObject,
                                                               UAbilitySystemComponent* ASC, ECharacterClass CharacterClass, int32 Level)
{
	UCharacterClassData* CharacterClassData = GetCharacterClassData(WorldContextObject);
	if (!CharacterClassData || !IsValid(ASC)) return;
	
	AActor* AvatarActor = ASC->GetAvatarActor();
	const FCharacterClassDefaultInfo CharacterClassDefaultInfo = CharacterClassData->FindCharacterClassInfo(CharacterClass);

	// 初始化角色默认等级属性

	// 设置主要属性
	FGameplayEffectContextHandle PrimaryAttributeContextHandle = ASC->MakeEffectContext();
	PrimaryAttributeContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassDefaultInfo.DefaultPrimaryEffects, Level, PrimaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());

	// 设置次要属性
	FGameplayEffectContextHandle SecondaryAttributeContextHandle = ASC->MakeEffectContext();
	SecondaryAttributeContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassData->DefaultSecondEffects, Level, SecondaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());

	// 设置生命值和法力值
	FGameplayEffectContextHandle BaseAttributeContextHande = ASC->MakeEffectContext();
	BaseAttributeContextHande.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle BaseAttributeSpecHandle = ASC->MakeOutgoingSpec(CharacterClassData->DefaultBaseEffects, Level, BaseAttributeContextHande);
	ASC->ApplyGameplayEffectSpecToSelf(*BaseAttributeSpecHandle.Data.Get());
	
}

void UMageAbilitySystemLibrary::GiveCharacterAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassData* CharacterClassData = GetCharacterClassData(WorldContextObject); // 获取到Mode中的角色默认技能
	if (!CharacterClassData || !IsValid(ASC)) return;
	
	for (const TSubclassOf<UGameplayAbility>& CommonAbility : CharacterClassData->CommonAbilities)
	{
		const FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(CommonAbility, 1);
		ASC->GiveAbility(AbilitySpec);
	}

	const FCharacterClassDefaultInfo& ClassDefaultInfo = CharacterClassData->FindCharacterClassInfo(CharacterClass);
	
	for (const TSubclassOf<UGameplayAbility>& DefaultAbility : ClassDefaultInfo.CharacterDefaultAbilites)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			int Level = ICombatInterface::Execute_GetCharacterLevel(ASC->GetAvatarActor());
			const FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(DefaultAbility, Level);
			ASC->GiveAbility(AbilitySpec);
		}
	}
	
}

UCharacterClassData* UMageAbilitySystemLibrary::GetCharacterClassData(const UObject* WorldContextObject)
{
	// 通过GameMode找到角色的默认属性
	AMageGameModeBase* MageGameModeBase = Cast<AMageGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (MageGameModeBase == nullptr) return nullptr;
	return MageGameModeBase->CharacterClassData;
}

UAbilityData* UMageAbilitySystemLibrary::GetAbilityData(const UObject* WorldContextObject)
{
	AMageGameModeBase* MageGameModeBase = Cast<AMageGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (MageGameModeBase == nullptr) return nullptr;
	return MageGameModeBase->AbilityData;
}

int32 UMageAbilitySystemLibrary::GetXPRewardForClassAndLevel(UObject* WorldContextObject, ECharacterClass CharacterClass, int32 Level)
{
	UCharacterClassData* CharacterClassData = GetCharacterClassData(WorldContextObject);
	if (!CharacterClassData) return 0;
	
	const FCharacterClassDefaultInfo CharacterClassDefaultInfo = CharacterClassData->FindCharacterClassInfo(CharacterClass);
	const int32 XPReward = CharacterClassDefaultInfo.XPRewardTable.GetValueAtLevel(Level);
	
	return XPReward;
}

bool UMageAbilitySystemLibrary::GetCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMageGameplayEffectContext* EffectContext = static_cast<const FMageGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetCriticalHit();
	}
	return false;
}

bool UMageAbilitySystemLibrary::GetBlockHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMageGameplayEffectContext* EffectContext = static_cast<const FMageGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetBlockHit();
	}
	return false;
}

bool UMageAbilitySystemLibrary::GetSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMageGameplayEffectContext* EffectContext = static_cast<const FMageGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetSuccessfulDebuff();
	}
	return false;
}

float UMageAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMageGameplayEffectContext* EffectContext = static_cast<const FMageGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float UMageAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMageGameplayEffectContext* EffectContext = static_cast<const FMageGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float UMageAbilitySystemLibrary::GetDebuffFrequence(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMageGameplayEffectContext* EffectContext = static_cast<const FMageGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetDebuffFrequence();
	}
	return 0.f;
}

FGameplayTag UMageAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMageGameplayEffectContext* EffectContext = static_cast<const FMageGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (EffectContext->GetDamageType().IsValid())
		{
			return *EffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

FVector UMageAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMageGameplayEffectContext* EffectContext = static_cast<const FMageGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UMageAbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMageGameplayEffectContext* EffectContext = static_cast<const FMageGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetKnockbackForce();
	}
	return FVector::ZeroVector;
}

void UMageAbilitySystemLibrary::SetCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bIsCriticalHit)
{
	if (FMageGameplayEffectContext* EffectContext = static_cast<FMageGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetCriticalHit(bIsCriticalHit);
	}
}

void UMageAbilitySystemLibrary::SetBlockHit(FGameplayEffectContextHandle& EffectContextHandle, bool bIsBlockHit)
{
	if (FMageGameplayEffectContext* EffectContext = static_cast<FMageGameplayEffectContext*>(EffectContextHandle.Get()))
    {
    	EffectContext->SetBlockHit(bIsBlockHit);
    }
}

void UMageAbilitySystemLibrary::SetSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle, bool bInSuccessfulDebuff)
{
	if (FMageGameplayEffectContext* EffectContext = static_cast<FMageGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetSuccessfulDebuff(bInSuccessfulDebuff);
	}
}

void UMageAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage)
{
	if (FMageGameplayEffectContext* EffectContext = static_cast<FMageGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetDebuffDamage(InDebuffDamage);
	}
}

void UMageAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration)
{
	if (FMageGameplayEffectContext* EffectContext = static_cast<FMageGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetDebuffDuration(InDebuffDuration);
	}
}

void UMageAbilitySystemLibrary::SetDebuffFrequence(FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequence)
{
	if (FMageGameplayEffectContext* EffectContext = static_cast<FMageGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetDebuffFrequence(InDebuffFrequence);
	}
}

void UMageAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle, FGameplayTag InDamageType)
{
	if (FMageGameplayEffectContext* EffectContext = static_cast<FMageGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		if (DamageType.IsValid())
		{
			EffectContext->SetDamageType(DamageType);
		}
	}
}

void UMageAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle, FVector InDeathImpulse)
{
	if (FMageGameplayEffectContext* EffectContext = static_cast<FMageGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetDeathImpulse(InDeathImpulse);
	}
}

void UMageAbilitySystemLibrary::SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle, FVector InKnockbackForce)
{
	if (FMageGameplayEffectContext* EffectContext = static_cast<FMageGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetKnockbackForce(InKnockbackForce);
	}
}

void UMageAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	if (!IsValid(DamageEffectParams.TargetAbilitySystemComponent)) return;
	
	const AActor* AvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	FGameplayEffectContextHandle ContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(AvatarActor);
	SetDeathImpulse(ContextHandle, DamageEffectParams.DeathImpulse); // 设置造成死亡后的冲击力
	SetKnockbackForce(ContextHandle, DamageEffectParams.KnockbackForce); // 设置造成击退的力度
	
	// 设置技能相关伤害属性
	FMageGameplayTags MageGameplayTags = FMageGameplayTags::Get();
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageEffectClass, DamageEffectParams.AbilityLevel, ContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, MageGameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, MageGameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, MageGameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, MageGameplayTags.Debuff_Frequence, DamageEffectParams.DebuffFrequence);
	
	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data); // 向目标应用游戏效果
}

void UMageAbilitySystemLibrary::GetAlivePlayerInSphereRadius(const UObject* WorldContextObject,
                                                             TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& InActorsToIgnore, float SphereRadius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams; // 碰撞查询的相关参数
	SphereParams.AddIgnoredActors(InActorsToIgnore); // 要忽略的Actor
	
	// 查询场景中我们击中了什么
	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		// 获取与球体碰撞的动态物体
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(SphereRadius), SphereParams);
		for (FOverlapResult& OverlapResult : Overlaps)
		{
			bool bIsImplementCombatInterface = OverlapResult.GetActor()->Implements<UCombatInterface>();
			// 实现了接口并且目标没有死亡
			if (bIsImplementCombatInterface && !ICombatInterface::Execute_IsDead(OverlapResult.GetActor()))
			{
				OutOverlappingActors.AddUnique(OverlapResult.GetActor());
			}
		}
	}
	
}

bool UMageAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	bool bPlayerFriend = FirstActor->ActorHasTag("Player") && SecondActor->ActorHasTag("Player");
	bool bEnemyFriend = FirstActor->ActorHasTag("Enemy") && SecondActor->ActorHasTag("Enemy");
	bool bFriend = bPlayerFriend || bEnemyFriend; // 是否是友方
	return !bFriend;
}