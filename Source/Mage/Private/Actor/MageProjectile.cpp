// Copyright AFstz.


#include "Actor/MageProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/MageAbilitySystemLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Mage/Mage.h"

AMageProjectile::AMageProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->InitialSpeed = 600.f;
	ProjectileMovement->MaxSpeed = 600.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AMageProjectile::Destroyed()
{
	PlayImpactEffect();
	Super::Destroyed();
}

void AMageProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	SetReplicateMovement(true);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlap);
	
	UGameplayStatics::SpawnSoundAttached(FlightingSound, GetRootComponent(), NAME_None, FVector(ForceInit), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);

}

bool AMageProjectile::IsValidOverlap(AActor* OtherActor)
{
	// 防止击中自己
	if (GetInstigator() == OtherActor) return false;
	// 防止击中队友
	if (!UMageAbilitySystemLibrary::IsNotFriend(GetInstigator(), OtherActor)) return false;
	
	return true;
}

void AMageProjectile::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;
	
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;
			if (bool bKnockbackSuccessful = DamageEffectParams.KnockbackChance >= FMath::RandRange(1, 100))
			{
				FRotator ProjectileRotation = GetActorForwardVector().Rotation();
				ProjectileRotation.Pitch = 45.f;
				const FVector ProjectileForceDirection = ProjectileRotation.Vector();
				
				const FVector KnockbackForce = ProjectileForceDirection * DamageEffectParams.KnockbackMagnitude;
				DamageEffectParams.KnockbackForce = KnockbackForce;
			}
			UMageAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		Destroy();
	}
}

void AMageProjectile::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void AMageProjectile::PlayImpactEffect()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactNiagara, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
}

