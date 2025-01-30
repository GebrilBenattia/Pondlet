// Fill out your copyright notice in the Description page of Project Settings.


#include "DayCycleSettings.h"
#include "TimeManager.h"

void UDayCycleSettings::GetSettingsByName(const FName& Name, FDayStateSettings& Output)
{
    for (const FDayStateSettings& Settings : DayStatesSettings)
    {
        if (Settings.DayStateName == Name)
        {
            Output = Settings; // Return the matching struct.
        }
    }
}

bool UDayCycleSettings::IsInsideInterval(const float& Time, const FDayStateSettings& Settings)
{
    float startTime = Settings.Start24HourFormat;
    float endTime = Settings.End24HourFormat;

    if (IsMidnightInInterval(startTime, endTime)) {
        return (Time >= startTime || Time < endTime);
    }

    return (Time >= startTime && Time < endTime);
}

//float UDayCycleSettings::ConvertTo24Hour(float Time, bool IsPM)
//{
//    // Add 12 hours if it's PM (except for 12 PM itself)
//    return IsPM ? Time + 12.0f : Time;
//}

bool UDayCycleSettings::IsMidnightInInterval(const float& Start, const float& End)
{
    return (Start > End);
}
