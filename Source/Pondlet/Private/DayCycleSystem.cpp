// Fill out your copyright notice in the Description page of Project Settings.


#include "DayCycleSystem.h"
#include "TimeManager.h"
#include "TimeManagerSettingsDataAsset.h"
#include "DayCycleSettings.h"

// Sets default values for this component's properties
ADayCycleSystem::ADayCycleSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;

	TimeManager = CreateDefaultSubobject<UTimeManager>(TEXT("TimeManager"));

	PendingTime = -1.f;
}

void ADayCycleSystem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Access values set in the Blueprint (class defaults)
	if (TimeManager && TimeManagerSettings)
	{
		TimeManager->Init(TimeManagerSettings);
	}
}

// Called when the game starts
void ADayCycleSystem::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void ADayCycleSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeManager->Update(DeltaTime);
}

