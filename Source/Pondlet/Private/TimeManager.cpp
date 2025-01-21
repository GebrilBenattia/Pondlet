// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeManager.h"
#include "TimeManagerSettingsDataAsset.h"

UTimeManager::UTimeManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTimeManager::Init(UTimeManagerSettingsDataAsset* Settings)
{
	TimeManagerSettings = Settings;
	TimeScale = Settings->TimeScale; //.005f for almost accurate time increment
	//IsPM = false;
	Day = 0;
	Time24Format = FMath::Fmod(Settings->StartValue24Format, 24.f);
	//Time12Format = FMath::Fmod(Settings->StartValue12Format, 12.f);
	Minutes = 0.f;//FMath::Fmod(Settings->StartValueMinutes, 60.f);

	if (Settings->bEnableSpecificTimeFormat) {
		/*if (Settings->bEnable12Format) {
			if (Settings->bStartAtPM) {
				TimeAngleFormat = (Time12Format * 15.f) + 180.f;
				IsPM = true;
			}
			else {
				TimeAngleFormat = Time12Format * 15.f;
				IsPM = false;
			}

			if (Settings->bEnableMinutes) {
				TimeAngleFormat += Minutes / 60.f;
			}
		}*/
		if (Settings->bEnable24Format) {
			TimeAngleFormat = Time24Format * 15.f;
			
			if (Settings->bEnableMinutes) {
				TimeAngleFormat += Minutes / 60.f;
			}
		}
	}
	else {
		TimeAngleFormat = 0.f;
	}

	Seconds = 0.f;
}

float UTimeManager::FromAngleToHour(const float& ToConvert)
{
	return ToConvert / 15.f;
}

float UTimeManager::FromAngleToMinutes(const float& ToConvert)
{
	return FromAngleToHour(ToConvert) * 60.f;
}

float UTimeManager::FromAngleToSeconds(const float& ToConvert)
{
	return FromAngleToMinutes(ToConvert) * 60.f;
}

float UTimeManager::From24ToAngle(const float& ToConvert)
{
	return ToConvert * 15.f;
}

//float UTimeManager::From12ToAngle(const float& ToConvert)
//{
//	return IsPM ? (ToConvert * 15.f) * 2.f : ToConvert * 15.f;
//}

float UTimeManager::FromMinutesToAngle(const float& ToConvert)
{
	return ToConvert / 60.f;
}

float UTimeManager::FromSecondsToAngle(const float& ToConvert)
{
	return ToConvert / 3600.f;
}

float UTimeManager::Get24FromAngle(const float& ToExtract)
{
	return FMath::Fmod(FromAngleToHour(ToExtract), 24.f);
}

//float UTimeManager::Get12FromAngle(const float& ToExtract)
//{
//	return FMath::Fmod(Get24FromAngle(ToExtract), 12.f);
//}

float UTimeManager::GetMinutesFromAngle(const float& ToExtract)
{
	return FMath::Fmod(ToExtract - FMath::Floor(FromAngleToHour(ToExtract)), 60.f);
}

float UTimeManager::GetSecondsFromAngle(const float& ToExtract)
{
	float minutes = GetMinutesFromAngle(ToExtract);

	return FMath::Fmod((minutes - FMath::Floor(minutes)) * 60.f, 60.f);
}

void UTimeManager::UpdateAngleFormat(float DeltaTime)
{
	TimeAngleFormat += DeltaTime * TimeScale * 1.f;

	if (TimeAngleFormat >= 360.f) {
		//IsPM = false;
		Day++;
		TimeAngleFormat = FMath::Fmod(TimeAngleFormat, 360.f);
	}
}

void UTimeManager::Update24Format(float DeltaTime)
{
	Time24Format = FMath::Fmod(TimeAngleFormat / 15.f, 24.f);
}

//void UTimeManager::Update12Format(float DeltaTime)
//{
//	Time12Format = FMath::Fmod(TimeAngleFormat / 15.f, 12.f);
//	if (Time12Format < 1.f) {
//		Time12Format += 12.f;
//		if (TimeAngleFormat > 180.f) {
//			IsPM = true;
//		}
//	}
//}

void UTimeManager::UpdateMinutes(float DeltaTime)
{
	float FractionalHours = 0.f;

	if (TimeManagerSettings->bEnable24Format)
		FractionalHours = (Time24Format - FMath::Floor(Time24Format));
	else 
		//FractionalHours = (Time12Format - FMath::Floor(Time12Format));

	Minutes = FMath::Fmod(FractionalHours * 60.f, 60.f); // BUG, Offset of 10e-3 * 60.f, break next computations.
}

void UTimeManager::UpdateSeconds(float DeltaTime)
{
	float FractionalMinutes = Minutes - FMath::Floor(Minutes);
	Seconds = FMath::Fmod(FractionalMinutes * 60.f, 60.f);
}

void UTimeManager::Update(float DeltaTime)
{
	UpdateAngleFormat(DeltaTime);

	if (TimeManagerSettings->bEnableSpecificTimeFormat) {
		if (TimeManagerSettings->bEnable24Format) {
			Update24Format(DeltaTime);

			if (TimeManagerSettings->bEnableMinutes) {
				UpdateMinutes(DeltaTime);

				if (TimeManagerSettings->bEnableSeconds)
					UpdateSeconds(DeltaTime);
			}
		}

		/*if (TimeManagerSettings->bEnable12Format) {
			Update12Format(DeltaTime);

			if (TimeManagerSettings->bEnableMinutes) {
				UpdateMinutes(DeltaTime);

				if (TimeManagerSettings->bEnableSeconds)
					UpdateSeconds(DeltaTime);
			}
		}*/
	}

	/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("bEnable12Format : %d"), TimeManagerSettings->bEnable12Format));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("bEnable24Format : %d"), TimeManagerSettings->bEnable24Format));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("bEnableSpecificHourFormat : %d"), TimeManagerSettings->bEnableSpecificTimeFormat));*/

	/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Seconds : %f"), Seconds));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Minutes : %f"), Minutes));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Time12Format : %f"), Time12Format));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Time24Format : %f"), Time24Format));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("TimeAngle : %f"), TimeAngleFormat));*/
}