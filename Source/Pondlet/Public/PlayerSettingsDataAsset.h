// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerSettingsDataAsset.generated.h"

UCLASS(BlueprintType)
class PONDLET_API UPlayerSettingsDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

    // Rotation speed
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings", meta = (ToolTip = "Defines how fast the rotation action occurs."))
    float Sensitivity;

    // Walk speed
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings", meta = (ToolTip = "Defines how fast the player is moving."))
    float MovementSpeed;

    // Air deceleration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings", meta = (ToolTip = "Defines how fast the player is decelerating after releasing movement input."))
    float BrakingAirDeceleration;

    // Zoom speed
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings", meta = (ToolTip = "Defines how fast the zoom action occurs."))
    float ZoomSpeed;

    // Field of View thresholds
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings", meta = (ToolTip = "Clamps FOV (zoom) value between these values. Min, Max"))
    FVector2D FOVThreshold;

    // Movement Speed Correction
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings", meta = (ToolTip = "Enables the movement speed correction based on the FOV (zoom)."))
    bool bEnableMovementSpeedCorrection;

    // Movement Speed Correction Factor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings", meta = (ToolTip = "Defines how the movement speed will be changed depending on the FOV (zoom)."), meta = (EditCondition = "bEnableMovementSpeedCorrection"))
    FVector2D MovementSpeedCorrectionThreshold;

    // Player Movement Boundaries
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings", meta = (ToolTip = "Enables the use of APlayerBoundaries (-> BP_PlayerBoundaries) to clamp player's position on X and Y axis. You need to drag one in your level and setup the Min and Max boundaries."))
    bool bEnablePlayerMovementBoundaries;

    // Field of View thresholds
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings", meta = (ToolTip = "[WORKING BUT NOT FINISHED] Shows cursor and enables mouse events."))
    bool bShowCursor;

    // Field of View thresholds
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings", meta = (ToolTip = "[NOT DONE YET](Could be done easly but prints are costly) Shows all relevant players info (some of values above)."))
    bool bDebugMode;
};
