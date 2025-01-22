// Fill out your copyright notice in the Description page of Project Settings.

#include "PondletPlayer.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBoundaries.h"
#include "PlayerSettingsDataAsset.h"

// Sets default values
APondletPlayer::APondletPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	SpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

// Called when the game starts or when spawned
void APondletPlayer::BeginPlay()
{
	Super::BeginPlay();

	ApplySettings();
}

// Called every frame
void APondletPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerSettings->bEnablePlayerMovementBoundaries)
		ClampPosition(MinMovementBoundary, MaxMovementBoundary);
}

// Called to bind functionality to input
void APondletPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		// Moving
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APondletPlayer::Move);

		// Enable Rotation
		EnhancedInputComponent->BindAction(EnableRotationAction, ETriggerEvent::Started, this, &APondletPlayer::EnableRotation);

		// Disable Rotation
		EnhancedInputComponent->BindAction(EnableRotationAction, ETriggerEvent::Completed, this, &APondletPlayer::EnableRotation);

		// Rotating
		EnhancedInputComponent->BindAction(RotationAction, ETriggerEvent::Triggered, this, &APondletPlayer::Rotate);

		// Zooming In
		EnhancedInputComponent->BindAction(ZoomInAction, ETriggerEvent::Triggered, this, &APondletPlayer::ZoomIn);

		// Zooming Out
		EnhancedInputComponent->BindAction(ZoomOutAction, ETriggerEvent::Triggered, this, &APondletPlayer::ZoomOut);
	}
	else
	{
		//UE_LOG(Err, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APondletPlayer::Move(const FInputActionValue& Value)
{
	if (Controller == nullptr)
		return;

	FVector2D MovementVector = PlayerSettings->bEnableMovementSpeedCorrection ? MovementSpeedCorrectionFactor * Value.Get<FVector2D>() : Value.Get<FVector2D>();

	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void APondletPlayer::EnableRotation(const FInputActionValue& Value)
{
	bIsRotationEnabled = !bIsRotationEnabled;
}

void APondletPlayer::Rotate(const FInputActionValue& Value)
{
	if (!bIsRotationEnabled)
		return;

	// input is a Vector2D
	float LookAxisValue = PlayerSettings->Sensitivity * Value.Get<float>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisValue);
	}
}

void APondletPlayer::ZoomIn(const FInputActionValue& Value)
{
	if (Controller == nullptr || Camera->FieldOfView == PlayerSettings->FOVThreshold.X)
		return;

	float ZoomOffset = PlayerSettings->ZoomSpeed * Value.Get<float>();
	Camera->SetFieldOfView(FMath::Clamp(Camera->FieldOfView - ZoomOffset, PlayerSettings->FOVThreshold.X, PlayerSettings->FOVThreshold.Y));

	if (PlayerSettings->bEnableMovementSpeedCorrection)
		UpdateMovementSpeedCorrectionFactor();
}

void APondletPlayer::ZoomOut(const FInputActionValue& Value)
{
	if (Controller == nullptr || Camera->FieldOfView == PlayerSettings->FOVThreshold.Y)
		return;

	float ZoomOffset = PlayerSettings->ZoomSpeed * Value.Get<float>();

	Camera->SetFieldOfView(FMath::Clamp(Camera->FieldOfView + ZoomOffset, PlayerSettings->FOVThreshold.X, PlayerSettings->FOVThreshold.Y));

	if (PlayerSettings->bEnableMovementSpeedCorrection)
		UpdateMovementSpeedCorrectionFactor();

}

void APondletPlayer::ApplySettings()
{
	GetCharacterMovement()->MaxWalkSpeed = PlayerSettings->MovementSpeed;
	GetCharacterMovement()->BrakingDecelerationFalling = PlayerSettings->BrakingAirDeceleration;

	/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("PlayerSettings Speed : %f"), PlayerSettings->MovementSpeed));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("CharacterMovement Speed : %f"), GetCharacterMovement()->MaxWalkSpeed));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("PlayerSettings AirBraking  : %f"), PlayerSettings->BrakingAirDeceleration));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("CharacterMovement AirBraking : %f"), GetCharacterMovement()->BrakingDecelerationFalling));*/

	if (PlayerSettings->bEnableMovementSpeedCorrection)
		UpdateMovementSpeedCorrectionFactor();

	if (PlayerSettings->bEnablePlayerMovementBoundaries)
		InitPlayerBoundaries();

	if (PlayerSettings->bShowCursor) {
		// Get the player controller
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			// Show the mouse cursor
			PlayerController->bShowMouseCursor = true;

			// Enable mouse events if required
			PlayerController->bEnableMouseOverEvents = true;
			PlayerController->bEnableClickEvents = true;

			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputMode);
		}
	}
}

void APondletPlayer::InitPlayerBoundaries()
{
	MovementBoundary = Cast<APlayerBoundaries>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerBoundaries::StaticClass()));

	if (!IsValid(MovementBoundary)) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Failed to get PlayerBoundaries instance.")));
		return;
	}

	MinMovementBoundary = MovementBoundary->MinBoundary;
	MaxMovementBoundary = MovementBoundary->MaxBoundary;

	if (!IsValid(MinMovementBoundary) && !IsValid(MaxMovementBoundary)) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Failed to get Min and Max on PlayerBoundaries instance.")));
		return;
	}
}

void APondletPlayer::ClampPosition(USceneComponent* MinPlayerBoundary, USceneComponent* MaxPlayerBoundary)
{
	if (MinPlayerBoundary && MaxPlayerBoundary)
	{
		if (!MovementBoundary->bIsCircle) {
			FVector MinBoundaryLocation = MinPlayerBoundary->GetComponentLocation();
			FVector MaxBoundaryLocation = MaxPlayerBoundary->GetComponentLocation();

			// Get the current player position
			FVector CurrentLocation = GetActorLocation();

			// Clamp the player's position
			FVector ClampedLocation;
			ClampedLocation.X = FMath::Clamp(CurrentLocation.X, MinBoundaryLocation.X, MaxBoundaryLocation.X);
			ClampedLocation.Y = FMath::Clamp(CurrentLocation.Y, MinBoundaryLocation.Y, MaxBoundaryLocation.Y);
			ClampedLocation.Z = CurrentLocation.Z;

			// Set the clamped position
			SetActorLocation(ClampedLocation);
		}

		else {
			// Get the center of the circle (MinPlayerBoundary)
			FVector CenterLocation = MinPlayerBoundary->GetComponentLocation();

			// Calculate the radius using the distance between Min and Max boundaries
			FVector MaxBoundaryLocation = MaxPlayerBoundary->GetComponentLocation();
			float Radius = FVector::Dist(CenterLocation, MaxBoundaryLocation);

			// Get the current player position
			FVector CurrentLocation = GetActorLocation();

			// Calculate the vector from the center to the player
			FVector DirectionToPlayer = CurrentLocation - CenterLocation;

			// Calculate the distance from the center to the player
			float DistanceToPlayer = DirectionToPlayer.Size();

			// If the distance exceeds the radius, clamp it
			if (DistanceToPlayer > Radius)
			{
				// Normalize the direction vector and scale it by the radius
				DirectionToPlayer = DirectionToPlayer.GetSafeNormal() * Radius;

				// Set the clamped position
				FVector ClampedLocation = CenterLocation + DirectionToPlayer;
				SetActorLocation(ClampedLocation);
			}
		}
	}
}

void APondletPlayer::UpdateMovementSpeedCorrectionFactor()
{
	// Calculate the percentage of the current FOV value within its threshold range
	float FOVPercentage = (Camera->FieldOfView - PlayerSettings->FOVThreshold.X) / (PlayerSettings->FOVThreshold.Y - PlayerSettings->FOVThreshold.X);

	// Clamp the FOV percentage between 0 and 1
	FOVPercentage = FMath::Clamp(FOVPercentage, 0.0f, 1.0f);

	// Now apply this percentage to the MovementSpeedCorrectionThreshold
	MovementSpeedCorrectionFactor = PlayerSettings->MovementSpeedCorrectionThreshold.X
		+ FOVPercentage * (PlayerSettings->MovementSpeedCorrectionThreshold.Y - PlayerSettings->MovementSpeedCorrectionThreshold.X);

	// Clamp the result to ensure it's within the MovementSpeedCorrectionThreshold range
	MovementSpeedCorrectionFactor = FMath::Clamp(MovementSpeedCorrectionFactor, PlayerSettings->MovementSpeedCorrectionThreshold.X, PlayerSettings->MovementSpeedCorrectionThreshold.Y);
}