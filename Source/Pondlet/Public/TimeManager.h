// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimeManager.generated.h"

class UTimeManagerSettingsDataAsset;

UCLASS()
class PONDLET_API UTimeManager : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time", meta = (AllowPrivateAccess = "true"))
	UTimeManagerSettingsDataAsset* TimeManagerSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
	int Day;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
	bool IsPM;*/
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
	float TimeScale;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
	float TimeAngleFormat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time", meta = (AllowPrivateAccess = "true"))
	float Time24Format;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time", meta = (AllowPrivateAccess = "true"))
	float Time12Format;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time", meta = (AllowPrivateAccess = "true"))
	float Minutes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time", meta = (AllowPrivateAccess = "true"))
	float Seconds;

public:

	UTimeManager();

	void Init(UTimeManagerSettingsDataAsset* Settings);
	
	UFUNCTION(BlueprintCallable)
	float FromAngleToHour(const float& ToConvert);

	UFUNCTION(BlueprintCallable)
	float FromAngleToMinutes(const float& ToConvert);

	UFUNCTION(BlueprintCallable)
	float FromAngleToSeconds(const float& ToConvert);
	
	UFUNCTION(BlueprintCallable)
	float From24ToAngle(const float& ToConvert);

	/*UFUNCTION(BlueprintCallable)
	float From12ToAngle(const float& ToConvert);*/

	UFUNCTION(BlueprintCallable)
	float FromMinutesToAngle(const float& ToConvert);

	UFUNCTION(BlueprintCallable)
	float FromSecondsToAngle(const float& ToConvert);

	UFUNCTION(BlueprintCallable)
	float Get24FromAngle(const float& ToExtract);

	/*UFUNCTION(BlueprintCallable)
	float Get12FromAngle(const float& ToExtract);*/
	
	UFUNCTION(BlueprintCallable)
	float GetMinutesFromAngle(const float& ToExtract);
	
	UFUNCTION(BlueprintCallable)
	float GetSecondsFromAngle(const float& ToExtract);

	void UpdateAngleFormat(float DeltaTime);
	void Update24Format(float DeltaTime);
	//void Update12Format(float DeltaTime);

	void UpdateMinutes(float DeltaTime);
	void UpdateSeconds(float DeltaTime);

	void Update(float DeltaTime);
};
