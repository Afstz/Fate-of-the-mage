// Copyright AFstz.

#include "AbilitySystem/MageAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "MageGameplayTags.h"
#include "AbilitySystem/MageAbilitySystemLibrary.h"
#include "AbilitySystem/Data/AbilityData.h"
#include "AbilitySystem/GameplayAbility/MageGameplayAbility.h"
#include "Interface/PlayerInterface.h"
#include "Mage/LogMageChannels.h"

void UMageAbilitySystemComponent::AbilityActorInfoIsSet()
{
	// 绑定的回调函数只在服务器执行
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::ClientOnEffectApplied);
}

void UMageAbilitySystemComponent::ClientOnEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer GameplayTagContainer;
	EffectSpec.GetAllAssetTags(GameplayTagContainer);
	EffectAssetTagsDelegate.Broadcast(GameplayTagContainer);
}

void UMageAbilitySystemComponent::AddCharacterAbilites(TArray<TSubclassOf<UGameplayAbility>>& AbilitiesClass)
{
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : AbilitiesClass)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (UMageGameplayAbility* MageGameplayAbility = Cast<UMageGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(MageGameplayAbility->StartupInputTag); // 把技能标签添加到动态标签容器中
			AbilitySpec.DynamicAbilityTags.AddTag(FMageGameplayTags::Get().Abilities_Status_Equipped);
			GiveAbility(AbilitySpec); // 赋予技能但不执行
		}
	}
	// 初始化技能完毕，给控制层发数据
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UMageAbilitySystemComponent::AddCharacterPassiveAbilites(TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilityClasses)
{
	for (const TSubclassOf<UGameplayAbility>& PassiveAbility : PassiveAbilityClasses)
	{
		FGameplayAbilitySpec PassiveAbilitySpec = FGameplayAbilitySpec(PassiveAbility);
		GiveAbilityAndActivateOnce(PassiveAbilitySpec);
	}
}

void UMageAbilitySystemComponent::AbilityInputPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ScopedAbilityListLock(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) // 获取所以可激活的技能数组
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) // 从动态标签容器中找是否有匹配标签
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive())
			{
				// 必须在技能激活的时候才有效
				// 如果有人正在监听，他们可能会将InputPressed事件复制到服务器。
				InvokeReplicatedEvent( 
					EAbilityGenericReplicatedEvent::InputPressed,
					AbilitySpec.Handle,
					AbilitySpec.ActivationInfo.GetActivationPredictionKey()
				);
			}
		}
	}
}

void UMageAbilitySystemComponent::AbilityInputHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ScopedAbilityListLock(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) // 获取所以可激活的技能数组
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) // 从动态标签容器中找是否有匹配标签
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive()) // 如果没有激活技能就尝试激活
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UMageAbilitySystemComponent::AbilityInputReleased(const FGameplayTag& GameplayTag)
{
	if (!GameplayTag.IsValid()) return;
	FScopedAbilityListLock ScopedAbilityListLock(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(GameplayTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
			if (AbilitySpec.IsActive())
			{
				InvokeReplicatedEvent( 
					EAbilityGenericReplicatedEvent::InputReleased,
					AbilitySpec.Handle,
					AbilitySpec.ActivationInfo.GetActivationPredictionKey()
				);
			}
		}
	}
}

void UMageAbilitySystemComponent::ForeachAbilitiesExecute(const FForeachAbilitiesSignature& ForeachDelegate)
{
	// 用于在我们遍历能力时，阻止我们从能力系统组件中移除能力(加锁,保证线程安全)
	FScopedAbilityListLock ScopedAbilityListLock(*this);
	
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!ForeachDelegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogMage, Error, TEXT("Failed to execute on [%hs] function"), __FUNCTION__);
		}
	}
}

FGameplayTag UMageAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (const FGameplayTag& AbilityTag : AbilitySpec.Ability->AbilityTags)
		{
			if (AbilityTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return AbilityTag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UMageAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (const FGameplayTag& Tag : AbilitySpec.DynamicAbilityTags) // DynamicAbilityTags存储着StartupInputTag
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Input"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UMageAbilitySystemComponent::GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (const FGameplayTag& StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UMageAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ScopedAbilityListLock(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (const FGameplayTag& Tag : AbilitySpec.Ability->AbilityTags)
		{
			if (AbilityTag.MatchesTagExact(Tag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

FGameplayAbilitySpec* UMageAbilitySystemComponent::GetSpecFromInputTag(const FGameplayTag& InputTag)
{
	FScopedAbilityListLock ScopedAbilityListLock(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

FGameplayTag UMageAbilitySystemComponent::GetStatusTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetStatusTagFromSpec(*AbilitySpec);
	}
	return FGameplayTag();
}

FGameplayTag UMageAbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*AbilitySpec);
	}
	return FGameplayTag();
}

void UMageAbilitySystemComponent::UpdateAbilityStatuses(const int32 InPlayerLevel)
{
	UAbilityData* AbilityData = UMageAbilitySystemLibrary::GetAbilityData(this);
	for (const FMageAbilityData& Data : AbilityData->AbilitiesData) // 寻找符合等级的技能
	{
		if (!Data.AbilityTag.IsValid()) continue;
		if (Data.AbilityLevelRequirement > InPlayerLevel) continue;
		
		if (GetSpecFromAbilityTag(Data.AbilityTag) == nullptr) // 判断是否赋予了这个技能
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Data.AbilityClass, 1.f);
			if (Data.AbilityTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Passive.Unupgradable"), false)))
			{
				AbilitySpec.DynamicAbilityTags.AddTag(FMageGameplayTags::Get().Abilities_Status_Unlocked);
			}
			else
			{
				AbilitySpec.DynamicAbilityTags.AddTag(FMageGameplayTags::Get().Abilities_Status_Eligible);
			}
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec); // 无需等待下次更新，直接复制到客户端
			const FGameplayTag StatusTag = GetStatusTagFromSpec(AbilitySpec);
			ClientUpdateAbilityStatus(Data.AbilityTag, StatusTag, 1.f);
		}
	}
}

void UMageAbilitySystemComponent::ServerSpendSkillPoint_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToSkillPoint(GetAvatarActor(), -1);
		}
		
		const FMageGameplayTags& MageGameplayTags = FMageGameplayTags::Get();
		FGameplayTag StatusTag = GetStatusTagFromSpec(*AbilitySpec);
		if (StatusTag.MatchesTagExact(MageGameplayTags.Abilities_Status_Eligible)) // 符合条件就解锁
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag(MageGameplayTags.Abilities_Status_Eligible);
			AbilitySpec->DynamicAbilityTags.AddTag(MageGameplayTags.Abilities_Status_Unlocked);
			StatusTag = GetStatusTagFromSpec(*AbilitySpec);
		}
		else if (StatusTag != MageGameplayTags.Abilities_Status_Locked)
		{
			AbilitySpec->Level += 1; // 已解锁就升级
		}
		ClientUpdateAbilityStatus(AbilityTag, StatusTag, AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec); // 对Spec进行修改需要直接网络同步更新
	}
}

void UMageAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
{
	AbilityStatusChanged.Broadcast(AbilityTag, StatusTag, NewLevel); // 同步技能状态
}

void UMageAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* AbilitySpec)
{
	const FGameplayTag InputTag = GetInputTagFromSpec(*AbilitySpec);
	AbilitySpec->DynamicAbilityTags.RemoveTag(InputTag);
	MarkAbilitySpecDirty(*AbilitySpec);
}

void UMageAbilitySystemComponent::ClearAbilitiesOfSlot(const FGameplayTag& InputTag)
{
	FScopedAbilityListLock ScopedAbilityListLock(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasInputTag(&AbilitySpec, InputTag))
		{
			ClearSlot(&AbilitySpec);
		}
	}
}

bool UMageAbilitySystemComponent::AbilityHasInputTag(const FGameplayAbilitySpec* AbilitySpec, const FGameplayTag& InputTag)
{
	if (AbilitySpec->DynamicAbilityTags.HasTag(InputTag))
	{
		return true;
	}
	return false;
}

bool UMageAbilitySystemComponent::SlotHasAbility(const FGameplayTag& InputTag)
{
	FScopedAbilityListLock ScopedAbilityListLock(*this);

	for (const FGameplayAbilitySpec& Spce : GetActivatableAbilities())
	{
		if (AbilityHasInputTag(&Spce, InputTag))
		{
			return true;
		}
	}
	return false;
}

bool UMageAbilitySystemComponent::IsPassiveAbility(const FGameplayAbilitySpec* AbilitySpec)
{
	return AbilitySpec->Ability->AbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Passive")));
}

bool UMageAbilitySystemComponent::AbilityHasAnyInputTag(const FGameplayAbilitySpec* AbilitySpec)
{
	return AbilitySpec->Ability->AbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Input")));
}

void UMageAbilitySystemComponent::AssignInputTagForAbility(FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& InputTag)
{
	ClearSlot(&AbilitySpec);
	AbilitySpec.DynamicAbilityTags.AddTag(InputTag);
}

void UMageAbilitySystemComponent::MulticastActivatePassiveEffect_Implementation(const FGameplayTag& PassiveTag, const bool bInActivate)
{
	OnPassiveActivationChanged.Broadcast(PassiveTag, bInActivate);
}

void UMageAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FMageGameplayTags& MageGameplayTags = FMageGameplayTags::Get();
		const FGameplayTag& PrevInputTag = GetInputTagFromSpec(*AbilitySpec);
		FGameplayTag StatusTag = GetStatusTagFromSpec(*AbilitySpec);

		const bool bStatusValid = StatusTag == MageGameplayTags.Abilities_Status_Unlocked || StatusTag == MageGameplayTags.Abilities_Status_Equipped;
		if (bStatusValid)
		{
			if (SlotHasAbility(InputTag)) // 对插槽已有的技能进行处理(被动技能取消激活 etc.)
			{
				FGameplayAbilitySpec* SpecWithSlot = GetSpecFromInputTag(InputTag);

				if (SpecWithSlot)
				{
					FGameplayTag PrevSpecAbilityTag = GetAbilityTagFromSpec(*SpecWithSlot);
					
					if (AbilityTag.MatchesTagExact(PrevSpecAbilityTag))
					{
						// 要装备的技能与插槽的技能相同,不进行处理
						ClientEquipAbility(AbilityTag, StatusTag, InputTag, PrevInputTag); 
						return;
					}

					if (IsPassiveAbility(SpecWithSlot)) // 插槽的被动技能结束
					{
						DeactivatePassiveDelegate.Broadcast(PrevSpecAbilityTag);
						MulticastActivatePassiveEffect(PrevSpecAbilityTag, false); // 多播关闭被动特效
					}

					ClearSlot(SpecWithSlot); // 清理插槽技能的InputTag
				}
			}
			
			if (IsPassiveAbility(AbilitySpec))
			{
				if (!AbilityHasAnyInputTag(AbilitySpec)) // 被动技能没有被赋予
				{
					TryActivateAbility(AbilitySpec->Handle);
					MulticastActivatePassiveEffect(AbilityTag, true);
				}
			}
			AssignInputTagForAbility(*AbilitySpec, InputTag); // 添加新的InputTag
			
			if (StatusTag.MatchesTagExact(MageGameplayTags.Abilities_Status_Unlocked))
			{
				// 改变技能状态为装备
				AbilitySpec->DynamicAbilityTags.RemoveTag(MageGameplayTags.Abilities_Status_Unlocked);
				AbilitySpec->DynamicAbilityTags.AddTag(MageGameplayTags.Abilities_Status_Equipped);
				StatusTag = MageGameplayTags.Abilities_Status_Equipped;
			}
			
			MarkAbilitySpecDirty(*AbilitySpec);
			ClientEquipAbility(AbilityTag, StatusTag, InputTag, PrevInputTag); // 更新客户端技能装备数据
		}
	}
}

void UMageAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& InputTag, const FGameplayTag& PrevInputTag)
{
	AbilityEquippedDelegate.Broadcast(AbilityTag, StatusTag, InputTag, PrevInputTag);
}

void UMageAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoint(GetAvatarActor()) > 0) // 防止发送垃圾数据
		{
			ServerUpgradeAttribute(AttributeTag); // 保证在服务器对属性进行操作
		}
	}
}

void UMageAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoint(GetAvatarActor()) > 0) // 服务器再次进行验证
		{
			FGameplayEventData Payload;
			Payload.EventTag = AttributeTag;
			Payload.EventMagnitude = 1.f;
	
			// 向自身发送事件，通过被动事件监听技能接收并给属性加点
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);
			
			IPlayerInterface::Execute_AddToAttributePoint(GetAvatarActor(), -1);
		}
	}
}

void UMageAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	// 技能在服务器上赋予所以得手动同步一下
	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}