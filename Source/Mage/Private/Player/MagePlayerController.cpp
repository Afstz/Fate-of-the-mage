// Copyright AFstz.

#include "Player/MagePlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "MageGameplayTags.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/MageAbilitySystemComponent.h"
#include "AbilitySystem/MageAbilitySystemLibrary.h"
#include "Actor/MagicCircle.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "Input/MageInputComponent.h"
#include "Interface/EnemyInterface.h"
#include "UI/Widget/MageUserWidget.h"
#include "GameFramework/Character.h"
#include "Interface/CombatInterface.h"
#include "Interface/HighlightInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Mage/Mage.h"
#include "UI/Widget/WidgetCompoent/DamageTextComponent.h"

AMagePlayerController::AMagePlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Comp"));
}

void AMagePlayerController::BeginPlay()
{
	Super::BeginPlay();
	// 检测是否有在蓝图中设置,如果没有就程序就会中断
	check(MageInputContext); 
	UEnhancedInputLocalPlayerSubsystem* EnhancedSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (EnhancedSubSystem) // 不能用Check因为服务器有非LocalPlayer
	{
		EnhancedSubSystem->AddMappingContext(MageInputContext, 0);
	}
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AMagePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRunning();
	UpdateMagicCircleLocation();
}

void AMagePlayerController::ServerPauseGame_Implementation(bool bShouldPause)
{
	UGameplayStatics::SetGamePaused(this, bShouldPause);
}

void AMagePlayerController::ShowDamageText_Implementation(ACharacter* TargetCharacter, float DamageValue, const bool bIsCriticalHit, const bool bIsBlockHit)
{
	// IsValid可以判断指针是否为空还可以判断指针标记为代销毁(最近一帧可能被调用了销毁)
	// DamageTextWidgetCompClass不用是因为只有空和不为空两种情况，因为我们在蓝图进行设置
	if (IsValid(TargetCharacter) && DamageTextWidgetCompClass)
	{
		UDamageTextComponent* DamageTextComponent = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextWidgetCompClass);
		DamageTextComponent->RegisterComponent(); // 动态的创建组件需要手动注册
		// 附加到角色身上保证字体在人物位
		DamageTextComponent->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		// 从角色身上分离，保证在一个位置播放完成动画不随父物体移动
		DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageTextComponent->SetDamageText(DamageValue, bIsCriticalHit, bIsBlockHit);
	}
}

void AMagePlayerController::CursorTrace()
{
	if (GetMageAbilitySystemComponent() && GetMageAbilitySystemComponent()->HasMatchingGameplayTag(FMageGameplayTags::Get().Block_Player_CursorTrace))
	{
		UnHighlightActor(CurrentCursorActor);
		UnHighlightActor(LastCursorActor);
		CurrentCursorActor = nullptr;
		LastCursorActor = nullptr;
		return;
	}
	
	// 释放贴花技能切换通道
	const ECollisionChannel TraceChannel = IsValid(MagicCircleDecal) ? ECC_MouseSkill : ECC_Visibility;
	
	GetHitResultUnderCursor(TraceChannel, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastCursorActor = CurrentCursorActor;
	if (IsValid(CursorHit.GetActor()) && CursorHit.GetActor()->Implements<UHighlightInterface>())
	{
		CurrentCursorActor = CursorHit.GetActor();
	}
	else
	{
		CurrentCursorActor = nullptr;
	}
	
	if (LastCursorActor != CurrentCursorActor)
	{
		HighlightActor(CurrentCursorActor);
		UnHighlightActor(LastCursorActor);
	}
}

void AMagePlayerController::HighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighlightInterface>())
	{
		IHighlightInterface::Execute_HighlightActor(InActor);
	}
}

void AMagePlayerController::UnHighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighlightInterface>())
	{
		IHighlightInterface::Execute_UnHighlightActor(InActor);
	}
}

void AMagePlayerController::AutoRunning()
{
	if (!bAutoRunning) return;
	if (GetMageAbilitySystemComponent() && GetMageAbilitySystemComponent()->HasMatchingGameplayTag(FMageGameplayTags::Get().Block_Player_AutoRun))
	{
		return;
	}
	
	if (APawn* ControlledPawn = GetPawn())
	{
		FVector Splineloc = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		// 给定位置寻找离样条曲线最近的切线方向
		FVector MoveDirection = Spline->FindDirectionClosestToWorldLocation(Splineloc, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(MoveDirection);
		
		float Destination = (Splineloc - CachedDestination).Length(); // 离目的地的距离
		if (Destination <= AutoRunAcceptanceRadius) // 到达阈值停止移动
		{
			bAutoRunning = false;
		}
	}
}

void AMagePlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (!IsValid(MagicCircleDecal))
	{
		MagicCircleDecal = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
		if (DecalMaterial)
		{
			MagicCircleDecal->MagicCircleDecalComponent->SetMaterial(0, DecalMaterial);
		}
	}
}

void AMagePlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircleDecal))
	{
		MagicCircleDecal->Destroy();
	}
}

void AMagePlayerController::UpdateMagicCircleLocation() const
{
	if (IsValid(MagicCircleDecal))
	{
		if (CursorHit.bBlockingHit)
		{
			MagicCircleDecal->SetActorLocation(CursorHit.ImpactPoint);
		}
	}
}

void AMagePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	// InputComponent在Project settings - input中默认把类设置成UEnhancedInputComponent
	UMageInputComponent* MageEnhancedInputComponent = CastChecked<UMageInputComponent>(InputComponent);
	
	MageEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMagePlayerController::Move);
	MageEnhancedInputComponent->BindAction(AttributeMenuAction, ETriggerEvent::Started, this, &AMagePlayerController::AttributeMenu);
	MageEnhancedInputComponent->BindAction(SkillMenuAction, ETriggerEvent::Started, this, &AMagePlayerController::SkillMenu);
	MageEnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AMagePlayerController::ShiftKeyPressed);
	MageEnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AMagePlayerController::ShiftKeyReleased);
	
	MageEnhancedInputComponent->BindAbilityActions(MageInputData, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagHeld, &ThisClass::AbilityInputTagReleased);
}

void AMagePlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetMageAbilitySystemComponent() && GetMageAbilitySystemComponent()->HasMatchingGameplayTag(FMageGameplayTags::Get().Block_Player_InputPressed))
	{
		return;
	}
	
	bAutoRunning = false;
	const FVector2D InputAxisValue = InputActionValue.Get<FVector2D>();
	// const FRotator Rotation = GetControlRotation(); // 默认 0 0 0
	// 只保留Yaw分量，其他分量设为0
	const FRotator YawRotation(0.f, 0.f, 0.f);
	
	// 将YawRotation转换成旋转矩阵，并从中提取X轴和Y轴方向向量
	// 因为YawRotaion始终为 0 0 0 , 变成标准的基向量组成的4x4矩阵
	//（UE默认的正方向为X轴）旋转矩阵前方向 X:1,0,0  右方向 Y:0,1,0
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisValue.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisValue.X);
	}
}

void AMagePlayerController::AttributeMenu(const FInputActionValue& InputActionValue)
{
	if (!IsValid(AttributeMenuWidget))
	{
		AttributeMenuWidget = CreateWidget<UMageUserWidget>(GetWorld(), AttributeMenuWidgetClass);
		if (AttributeMenuWidget)
		{
			AttributeMenuWidget->AddToViewport(1); // 防止其他UI覆盖
			AttributeMenuWidget->SetPositionInViewport(FVector2D(0, 20.f));
		}
	}
	else
	{
		if (AttributeMenuWidget)
		{
			AttributeMenuWidget->RemoveFromParent();
			AttributeMenuWidget = nullptr;
		}
	}
}

void AMagePlayerController::SetAttributeMenuWidget(UMageUserWidget* InAttributeMenu)
{
	AttributeMenuWidget = InAttributeMenu;
}

void AMagePlayerController::SkillMenu(const FInputActionValue& InputActionValue)
{
	if (!IsValid(SkillMenuWidget))
	{
		SkillMenuWidget = CreateWidget<UMageUserWidget>(GetWorld(), SkillMenuWidgetClass);
		if (SkillMenuWidget)
		{
			SkillMenuWidget->AddToViewport(1); // 防止其他UI覆盖
			SkillMenuWidget->ForceLayoutPrepass(); // 强制布局刷新以获取Widget大小
			float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(this); // 获取视口的缩放比例
			FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this) / ViewportScale; // 物理像素转逻辑坐标
			FVector2D WidgetSize = SkillMenuWidget->GetDesiredSize();
			float FinalPositionX = ViewportSize.X - WidgetSize.X;
			SkillMenuWidget->SetPositionInViewport(FVector2D(FinalPositionX, 25.f), false);
		}
	}
	else
	{
		if (SkillMenuWidget)
		{
			SkillMenuWidget->RemoveFromParent();
			SkillMenuWidget = nullptr;
		}
	}
}

void AMagePlayerController::SetSkillMenuWidget(UMageUserWidget* InSkillMenu)
{
	SkillMenuWidget = InSkillMenu;
}

void AMagePlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (UMageAbilitySystemLibrary::IsCharacterDead(GetCharacter())) return;
	
	if (GetMageAbilitySystemComponent() && GetMageAbilitySystemComponent()->HasMatchingGameplayTag(FMageGameplayTags::Get().Block_Player_InputPressed))
	{
		return;
	}
	
	if (InputTag.MatchesTagExact(FMageGameplayTags::Get().Input_LMB))
	{
		if (IsValid(CurrentCursorActor) && CurrentCursorActor->Implements<UEnemyInterface>())
		{
			TargetingStatus = ETargetingStatus::TargetingEnemy;
		}
		else
		{
			TargetingStatus = ETargetingStatus::NoneTarget;
		}
	}
	bAutoRunning = false;
	HoldingTime = 0.f; // 重置自动移动按住时间

	if (GetMageAbilitySystemComponent())
	{
		GetMageAbilitySystemComponent()->AbilityInputPressed(InputTag);
	}
}

void AMagePlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (UMageAbilitySystemLibrary::IsCharacterDead(GetCharacter())) return;
	
	if (GetMageAbilitySystemComponent() && GetMageAbilitySystemComponent()->HasMatchingGameplayTag(FMageGameplayTags::Get().Block_Player_InputHeld))
	{
		return;
	}
	
	if (!InputTag.MatchesTagExact(FMageGameplayTags::Get().Input_LMB))
	{
		if (GetMageAbilitySystemComponent())
		{
			GetMageAbilitySystemComponent()->AbilityInputHeld(InputTag);
		}
		return;
	}
	
	if (TargetingStatus == ETargetingStatus::TargetingEnemy || bShiftKeyPressed)
	{
		// 左键鼠标指向敌人或者按住Shift,执行技能逻辑
		if (GetMageAbilitySystemComponent())
		{
			GetMageAbilitySystemComponent()->AbilityInputHeld(InputTag);
		}
	}
	else // 长按移动
	{
		HoldingTime += GetWorld()->GetDeltaSeconds(); // 累加时间判断是否要自动移动
		
		if (APawn* ControlledPawn = GetPawn())
		{
			// 没有获取到目的地就原地不动
			CachedDestination = CursorHit.bBlockingHit ? CursorHit.ImpactPoint : ControlledPawn->GetActorLocation();
			// 获取要移动目的地的方向
			FVector MoveDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			
			ControlledPawn->AddMovementInput(MoveDirection);
		}
	}
}

void AMagePlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (UMageAbilitySystemLibrary::IsCharacterDead(GetCharacter())) return;
	
	if (GetMageAbilitySystemComponent() && GetMageAbilitySystemComponent()->HasMatchingGameplayTag(FMageGameplayTags::Get().Block_Player_InputReleased))
	{
		return;
	}
	
	if (!InputTag.MatchesTagExact(FMageGameplayTags::Get().Input_LMB))
	{
		if (GetMageAbilitySystemComponent())
		{
			GetMageAbilitySystemComponent()->AbilityInputReleased(InputTag);
		}
		return;
	}

	if (GetMageAbilitySystemComponent()) // 每次释放按键都要执行Released
	{
		GetMageAbilitySystemComponent()->AbilityInputReleased(InputTag);
	}

	if (TargetingStatus != ETargetingStatus::TargetingEnemy && !bShiftKeyPressed)
	{
		// 如果是存档点就更改目的地到指定位置
		if (IsValid(CurrentCursorActor) && CurrentCursorActor->Implements<UHighlightInterface>())
		{
			IHighlightInterface::Execute_MoveToLocation(CurrentCursorActor, CachedDestination);
		}
		else if (GetMageAbilitySystemComponent() && !GetMageAbilitySystemComponent()->HasMatchingGameplayTag(FMageGameplayTags::Get().Block_Player_InputPressed))
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, MouseClickEffect, CachedDestination);
		}
		
		APawn* ControlledPawn = GetPawn();
		if (HoldingTime <= ShortPressThreshold && ControlledPawn) // 小于短按阈值则进行自动移动
		{
			Spline->ClearSplinePoints(); // 清除样条线已有的点,重新计算
			UNavigationPath* FoundPath = UNavigationSystemV1::FindPathToLocationSynchronously(
				this, ControlledPawn->GetActorLocation(), CachedDestination);
			
			if (FoundPath && FoundPath->PathPoints.Num() > 0)
			{
				for (const FVector& PathPoint : FoundPath->PathPoints)
				{
					Spline->AddSplinePoint(PathPoint, ESplineCoordinateSpace::World); // 添加已找到的路径
					// DrawDebugSphere(GetWorld(), PathPoint, 8.f, 8, FColor::Orange, false, 5.f);
				}
				// 以防缓存的目的地在达不到的地方,修改为最后一个路径点
				CachedDestination = FoundPath->PathPoints[FoundPath->PathPoints.Num() - 1];
				bAutoRunning = true; // 开始自动移动
			}
		}
	}
}

UMageAbilitySystemComponent* AMagePlayerController::GetMageAbilitySystemComponent()
{
	if (MageASC == nullptr)
	{
		MageASC = Cast<UMageAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return MageASC;
}




