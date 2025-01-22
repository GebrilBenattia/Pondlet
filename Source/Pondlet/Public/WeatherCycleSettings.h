// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeatherCycleSettings.generated.h"


USTRUCT(BlueprintType)
struct FWeatherStateSettings
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather", meta = (AllowPrivateAccess = "true"))
	FName WeatherStateName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather", meta = (AllowPrivateAccess = "true"))
	float WindIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather", meta = (AllowPrivateAccess = "true"))
	float FallingLeavesIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather", meta = (AllowPrivateAccess = "true"))
	float CloudContrast;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather", meta = (AllowPrivateAccess = "true"))
	float FoliageGrowthScale;
};
/**
 * 
 */
UCLASS()
class PONDLET_API UWeatherCycleSettings : public UPrimaryDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day States", meta = (AllowPrivateAccess = "true"))
	TArray<FWeatherStateSettings> WeatherStatesSettings;


public : 
	UFUNCTION(BlueprintCallable)
	void GetSettingsByName(const FName& Name, FWeatherStateSettings& Output);
	
};
