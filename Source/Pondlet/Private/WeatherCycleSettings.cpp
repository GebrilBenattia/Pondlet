// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherCycleSettings.h"

void UWeatherCycleSettings::GetSettingsByName(const FName& Name, FWeatherStateSettings& Output)
{
    for (const FWeatherStateSettings& Settings : WeatherStatesSettings)
    {
        if (Settings.WeatherStateName == Name)
        {
            Output = Settings; // Return the matching struct.
        }
    }
}
