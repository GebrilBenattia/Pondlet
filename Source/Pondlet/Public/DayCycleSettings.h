// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DayCycleSettings.generated.h"

class TimeManager;

USTRUCT(BlueprintType)
struct FDayStateSettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
    FName DayStateName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
    float Start24HourFormat;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
    float End24HourFormat;

    /*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
    float Start12HourFormat;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
    bool IsStartPM;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
    float End12HourFormat;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
    bool IsEndPM;*/
};

UCLASS(BlueprintType)
class PONDLET_API UDayCycleSettings : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day States", meta = (AllowPrivateAccess = "true"))
    TArray<FDayStateSettings> DayStatesSettings;

    UFUNCTION(BlueprintCallable)
    void GetSettingsByName(const FName& Name, FDayStateSettings& Output);

    UFUNCTION(BlueprintCallable)
    bool IsInsideInterval(const float& Time, const FDayStateSettings& Settings);
    //bool IsInsideInterval(bool Is24Format, bool IsPM, const float& Time, const FDayStateSettings& Settings);

    //float ConvertTo24Hour(float Time, bool IsPM);

    UFUNCTION(BlueprintCallable)
    bool IsMidnightInInterval(const float& Start, const float& End);
};
