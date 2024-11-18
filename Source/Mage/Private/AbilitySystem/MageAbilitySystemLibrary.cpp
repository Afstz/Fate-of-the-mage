// Copyright AFstz.


#include "AbilitySystem/MageAbilitySystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "MageAbilityTypes.h"
#include "Engine/OverlapResult.h"
#include "Game/MageGameModeBase.h"
#include "Interface/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MagePlayerState.h"
#include "UI/HUD/MageHUD.h"
#include "UI/WidgetController/MageWidgetController.h"

UOverlayWidgetController* UMageAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	// 根据提供的世界上下文信息找到当前世界的全部PlayerController, 0 为本地控制的角色
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AMageHUD* MageHUD = Cast<AMageHUD>(PC->GetHUD()))
		{
			AMagePlayerState* PS = PC->GetPlayerState<AMagePlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return MageHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr; 
}

UAttributeMenuWidgetController* UMageAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AMageHUD* MageHUD = Cast<AMageHUD>(PC->GetHUD()))
		{
			AMagePlayerState* PS = PC->GetPlayerState<AMagePlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return MageHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
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

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor()))
	{
		for (const TSubclassOf<UGameplayAbility>& DefaultAbility : ClassDefaultInfo.CharacterDefaultAbilites)
		{
			const FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(DefaultAbility, CombatInterface->GetCharacterLevel());
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

bool UMageAbilitySystemLibrary::GetIsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMageGameplayEffectContext* EffectContext = static_cast<const FMageGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetIsCriticalHit();
	}
	return false;
}

bool UMageAbilitySystemLibrary::GetIsBlockHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMageGameplayEffectContext* EffectContext = static_cast<const FMageGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EffectContext->GetIsBlockHit();
	}
	return false;
}

void UMageAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bIsCriticalHit)
{
	if (FMageGameplayEffectContext* EffectContext = static_cast<FMageGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EffectContext->SetIsCriticalHit(bIsCriticalHit);
	}
}

void UMageAbilitySystemLibrary::SetIsBlockHit(FGameplayEffectContextHandle& EffectContextHandle, bool bIsBlockHit)
{
	if (FMageGameplayEffectContext* EffectContext = static_cast<FMageGameplayEffectContext*>(EffectContextHandle.Get()))
    {
    	EffectContext->SetIsBlockHit(bIsBlockHit);
    }
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
	bool IsPlayerFriend = FirstActor->ActorHasTag("Player") && SecondActor->ActorHasTag("Player");
	bool IsEnemyFriend = FirstActor->ActorHasTag("Enemy") && SecondActor->ActorHasTag("Enemy");
	bool bIsFriend = IsPlayerFriend || IsEnemyFriend; // 是否是友方
	return !bIsFriend;
}