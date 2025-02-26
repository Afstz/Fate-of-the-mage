// Copyright AFstz.


#include "Actor/PointCollection.h"

#include "AbilitySystem/MageAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

APointCollection::APointCollection()
{
	PrimaryActorTick.bCanEverTick = false;

	Pt_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_Root"));
	SetRootComponent(Pt_Root);
	ImmutablePoints.Add(Pt_Root);
	Pt_1 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_1"));
	Pt_1->SetupAttachment(RootComponent);
	ImmutablePoints.Add(Pt_1);
	Pt_2 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_2"));
	Pt_2->SetupAttachment(RootComponent);
	ImmutablePoints.Add(Pt_2);
	Pt_3 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_3"));
	Pt_3->SetupAttachment(RootComponent);
	ImmutablePoints.Add(Pt_3);
	Pt_4 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_4"));
	Pt_4->SetupAttachment(RootComponent);
	ImmutablePoints.Add(Pt_4);
	Pt_5 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_5"));
	Pt_5->SetupAttachment(RootComponent);
	ImmutablePoints.Add(Pt_5);
	Pt_6 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_6"));
	Pt_6->SetupAttachment(RootComponent);
	ImmutablePoints.Add(Pt_6);
	Pt_7 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_7"));
	Pt_7->SetupAttachment(RootComponent);
	ImmutablePoints.Add(Pt_7);
	Pt_8 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_8"));
	Pt_8->SetupAttachment(RootComponent);
	ImmutablePoints.Add(Pt_8);
	Pt_9 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_9"));
	Pt_9->SetupAttachment(RootComponent);
	ImmutablePoints.Add(Pt_9);
	Pt_10 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_10"));
	Pt_10->SetupAttachment(RootComponent);
	ImmutablePoints.Add(Pt_10);
}

void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}

void APointCollection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& PointCenter, int32 NumPoints, float YawOverride)
{
	checkf(ImmutablePoints.Num() >= NumPoints, TEXT("ImmutablePoints Array Out of Bounds."))
	TArray<USceneComponent*> ValidPoints;
	
	for (USceneComponent* Pt : ImmutablePoints)
	{
		if (ValidPoints.Num() > NumPoints) return ValidPoints;

		if (Pt != Pt_Root)
		{
			FVector RootToPoint = Pt->GetComponentLocation() - Pt_Root->GetComponentLocation();
			RootToPoint = RootToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			Pt->SetWorldLocation(Pt_Root->GetComponentLocation() + RootToPoint);
		}

		const FVector PointLocation = Pt->GetComponentLocation();
		const FVector RaisedLocation = FVector(PointLocation.X, PointLocation.Y, PointLocation.Z + 300.f);
		const FVector LoweredLocation = FVector(PointLocation.X, PointLocation.Y, PointLocation.Z - 300.f);
		
		TArray<AActor*> IgnoreActors; // 忽略的角色
		UMageAbilitySystemLibrary::GetAlivePlayerInSphereRadius(this, IgnoreActors, TArray<AActor*>(), 500.f, GetActorLocation());

		FHitResult HitResult;
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActors(IgnoreActors);
		// 获取击中的静态物体位置
		GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, FName("BlockAll"), CollisionQueryParams);
		
		if (HitResult.bBlockingHit)
		{
			const FVector AdjustedLocation = FVector(PointLocation.X, PointLocation.Y, HitResult.ImpactPoint.Z);
			Pt->SetWorldLocation(AdjustedLocation);
			Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.Normal)); // 根据法线旋转
		}
		
		ValidPoints.Add(Pt);
	}
	return ValidPoints;
}

