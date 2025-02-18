// Copyright AFstz.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Interface/CombatInterface.h"
#include "GameFramework/Character.h"
#include "MageCharacterBase.generated.h"

class UPassiveNiagaraComponent;
struct FGameplayTag;
class UDebuffNiagaraComponent;
class UNiagaraSystem;
class UMotionWarpingComponent;
class UGameplayAbility;
class UAttributeSet;
class UGameplayEffect;

UCLASS(Abstract)
class MAGE_API AMageCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AMageCharacterBase();
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
	/** Combat Interface */
	virtual FVector GetLocationByWeaponSocket_Implementation() const override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() const override;
	virtual void Die(const FVector& InDeathImpulse) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatarActor_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual int32 GetMinionCount_Implementation() const override;
	virtual void AddMinionCount_Implementation(const int32 InMinionCount) override;
	virtual ECharacterClass GetCharacterClass_Implementation() const override;
	virtual USkeletalMeshComponent* GetWeaponMesh_Implementation() const override;
	virtual FASCRegisteredSignature& GetASCRegisteredDelegate() override;
	virtual FOnDeathSignature& GetOnDeathDelegate() override;
	virtual FDamageDelegate& GetDamageDelegate() override;
	virtual void SetBeingShocked_Implementation(bool InBeingShocked) override;
	virtual bool IsBeingShocked_Implementation() const override;
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MultiHandleDeath(const FVector& DeathImpulse);
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void InitAbilityActorInfo();
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	/** Combat */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UNiagaraSystem> BloodEffect;
	bool bDead = false; // 角色是否死亡
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USoundBase> DeathSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Attributes")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 600.f;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stunned, Category = "Combat")
	bool bStunned = false; // 眩晕状态
	UFUNCTION()
	virtual void OnRep_Stunned(bool OldStunned);
	virtual void OnStunTagChanged(const FGameplayTag StunTag, int32 NewCount);
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Combat")
	bool bBeingShocked = false;

	/** Init Attribute */
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultBaseEffects;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryEffects;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryEffects;
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect>& InitAttributeEffects, float Level);
	virtual void InitDefaultAttributes();

	/** Dissolve Material*/
	UPROPERTY(EditDefaultsOnly, Category = "Dissolve")
	TObjectPtr<UMaterialInstance> CharacterMaterialInst;
	UPROPERTY(EditDefaultsOnly, Category = "Dissolve")
	TObjectPtr<UMaterialInstance> WeaponMaterialInst;
	void Dissolve();
	UFUNCTION(BlueprintImplementableEvent, Category = "Dissolve")
	void StartDissolveTimeLine(const TArray<UMaterialInstanceDynamic*>& MaterialInstDynamics);

	void AddCharacterAbilites();
	
	/** Minion */
	UPROPERTY(EditAnywhere, Category = "Minion")
	int32 MinionCount = 0;

	/** Combat Interface Delegate */
	
	// ASC初始化时委托
	FASCRegisteredSignature ASCRegisteredDelegate;
	
	// Actor死亡时委托
	FOnDeathSignature OnDeathDelegate;

	// TakeDamage受到伤害时委托
	FDamageDelegate DamageDelegate;
	
	/** Debuff Component*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> BurnNiagaraComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> StunNiagaraComponent;
private:
	/** Startup */
	UPROPERTY(EditDefaultsOnly, Category = "Abilites")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilites;
	UPROPERTY(EditDefaultsOnly, Category = "Abilites")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilites;

	/** Passive Effect*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EffectAttachComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> HaloOfProtectionNiagaraComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> LifeRegenerateNiagaraComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> ManaRegenerateNiagaraComponent;
};


