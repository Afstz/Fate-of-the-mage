// Copyright AFstz.


#include "Player/MagePlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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
	check(EnhancedSubSystem);
	EnhancedSubSystem->AddMappingContext(MageInputContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
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
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisValue.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisValue.X);
	}
}