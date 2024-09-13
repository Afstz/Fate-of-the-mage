// Copyright AFstz.


#include "Player/MagePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interface/EnemyInterface.h"

AMagePlayerController::AMagePlayerController()
{
	bReplicates = true;
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
}

void AMagePlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = CurrentActor;
	CurrentActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	if (LastActor == nullptr)
	{
		if (CurrentActor == nullptr)
		{
			// Do nothing.
		}
		else
		{
			CurrentActor->HighlightActor();
		}
	}
	else
	{
		if (CurrentActor == nullptr)
		{
			LastActor->UnHighlightActor();
		}
		else
		{
			if (LastActor != CurrentActor)
			{
				LastActor->UnHighlightActor();
				CurrentActor->HighlightActor();
			}
			else
			{
				// Do nothing.
			}
		}
	}
}

void AMagePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 尝试转换并断言
	// InputComponent在Project settings - input中默认把类设置成UEnhancedInputComponent
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMagePlayerController::Move);
}

void AMagePlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisValue = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation(); // 默认 0 0 0
	// 只保留Yaw分量，其他分量设为0
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	// 将YawRotation转换成旋转矩阵，并从中提取X轴和Y轴方向向量
	// 因为YawRotaion始终为 0 0 0 , 变成标准的基向量组成的4x4矩阵
	// （UE默认的正方向为X轴）旋转矩阵前方向 X:1,0,0  右方向 Y:0,1,0
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisValue.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisValue.X);
	}
}

