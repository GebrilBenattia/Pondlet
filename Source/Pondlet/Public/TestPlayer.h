// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Containers/StaticArray.h"
#include "TestPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class AStaticMeshActor;
class APlayerBoundaries;
class UPlayerSettingsDataAsset;

UCLASS()
class PONDLET_API ATestPlayer : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Movement Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MovementAction;

	/** Enable Rotation Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* EnableRotationAction;

	/** Rotation Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* RotationAction;

	/** ZoomIn Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ZoomInAction;

	/** ZoomOut Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ZoomOutAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	UPlayerSettingsDataAsset* PlayerSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	APlayerBoundaries* MovementBoundary;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MinMovementBoundary;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MaxMovementBoundary;

	bool bIsRotationEnabled;
	float MovementSpeedCorrectionFactor;

public:
	// Sets default values for this character's properties
	ATestPlayer();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Inputs
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Move(const FInputActionValue& Value);
	void EnableRotation(const FInputActionValue& Value);
	void Rotate(const FInputActionValue& Value);
	void ZoomIn(const FInputActionValue& Value);
	void ZoomOut(const FInputActionValue& Value);

	//Movements
	void ApplySettings();
	void InitPlayerBoundaries();
	void ClampPosition(USceneComponent* MinPlayerBoundary, USceneComponent* MaxPlayerBoundary);
	void UpdateMovementSpeedCorrectionFactor();
};
