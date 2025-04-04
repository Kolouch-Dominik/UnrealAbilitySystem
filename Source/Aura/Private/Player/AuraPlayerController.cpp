#include "Player/AuraPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaSeconds)
{
	Super::PlayerTick(DeltaSeconds);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/**
	 * Line trace from cursor. There are several scenarions:
	 * A. Last actor is null and This actor is null
	 *		- Do nothing
	 * B. Last actor is null but This actor is valid
	 *		- Higlight this actor
	 * C. Last actor is valid and This actor is null
	 *		- Unhighlight last actor
	 * D. Both actors are valid, but Last actor is not equal to This actor (they are not same)
	 *		- Unhighlight last actor and Higlight this actor
	 * E. Last actor is This actor
	 *		- Do nothing
	 ***/

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// Case B
			ThisActor->HiglightActor();
		}
		else
		{
			// Case A - Do nothing
		}
	}
	else // Last actor is valid
	{
		if (ThisActor == nullptr)
		{
			// Case C
			LastActor->UnhiglightActor();
		}
		else // both are valid
		{
			if (LastActor != ThisActor)
			{
				// Case D
				LastActor->UnhiglightActor();
				ThisActor->HiglightActor();
			}
			else
			{
				// Case E - Do nothing
			}
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	
	if (SubSystem)
	{
		SubSystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.0f);

	const FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* PawnController = GetPawn<APawn>())
	{
		PawnController->AddMovementInput(FowardDirection, InputAxisVector.Y);
		PawnController->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}
