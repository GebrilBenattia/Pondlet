// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TimeManagerSettingsDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PONDLET_API UTimeManagerSettingsDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

    // Time Scale
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Settings", meta = (ToolTip = "Defines how fast the time flies."))
    float TimeScale;

    // Specific Time Format
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Settings", meta = (ToolTip = "Enables other time formats selection (default is a 360 degrees angle)."))
    bool bEnableSpecificTimeFormat;

    // 24h time format
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Settings", meta = (ToolTip = "Enables 24h time format calculation."), meta = (EditCondition = "bEnableSpecificTimeFormat"))
    bool bEnable24Format;

    // Start Value 24 Format
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Settings", meta = (ToolTip = "Defines the default time value at the start for 24h time format."), meta = (EditCondition = "bEnableSpecificTimeFormat && bEnable24Format"))
    float StartValue24Format;

    //// 12h time format
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Settings", meta = (ToolTip = "Enables 12h time format calculation."), meta = (EditCondition = "bEnableSpecificTimeFormat"))
    //bool bEnable12Format;

    //// 12h time format
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Settings", meta = (ToolTip = "Converts hour to directly start at PM."), meta = (EditCondition = "bEnableSpecificTimeFormat && bEnable12Format"))
    //bool bStartAtPM;

    //// Start Value 12 Format
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Settings", meta = (ToolTip = "Defines the default time value at the start for 12h time format."), meta = (EditCondition = "bEnableSpecificTimeFormat && bEnable12Format"))
    //float StartValue12Format;

    // Specific Time Format
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Settings", meta = (ToolTip = "Enables minutes calculation."), meta = (EditCondition = "bEnableSpecificTimeFormat && bEnable24Format || bEnable12Format"))
    bool bEnableMinutes;

    //// Start Value Minutes
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Settings", meta = (ToolTip = "Defines the default time value at the start for minutes."), meta = (EditCondition = "bEnableSpecificTimeFormat && bEnableMinutes"))
    //float StartValueMinutes;

    // Specific Time Format
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Settings", meta = (ToolTip = "Enables seconds calculation."), meta = (EditCondition = "bEnableSpecificTimeFormat && (bEnable24Format || bEnable12Format) && bEnableMinutes"))
    bool bEnableSeconds;

public:

    //// Called when a property is changed in the editor
    //virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
