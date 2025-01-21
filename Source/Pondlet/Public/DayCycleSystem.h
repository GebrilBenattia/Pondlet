// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DayCycleSystem.generated.h"

class UTimeManager;
class UTimeManagerSettingsDataAsset;
class UDayCycleSettings;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PONDLET_API ADayCycleSystem : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
	UTimeManager* TimeManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
	UTimeManagerSettingsDataAsset* TimeManagerSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayCycleSettings", meta = (AllowPrivateAccess = "true"))
	UDayCycleSettings* DayCycleSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "true"))
	float PendingTime;

public:	
	// Sets default values for this component's properties
	ADayCycleSystem();

protected:

	virtual void PostInitializeComponents() override;

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
