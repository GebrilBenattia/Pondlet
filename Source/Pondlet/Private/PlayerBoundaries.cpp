// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBoundaries.h"

// Sets default values
APlayerBoundaries::APlayerBoundaries()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and set the Default Scene Root Component
	DefaultSceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRootComponent;  // Set it as the root component

	// Create and attach the MinBoundaryMesh
	MinBoundary = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MinBoundaryMesh"));
	MinBoundary->SetupAttachment(RootComponent);
	MinBoundary->bHiddenInGame = true;

	// Create and attach the MaxBoundaryMesh
	MaxBoundary = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MaxBoundaryMesh"));
	MaxBoundary->SetupAttachment(RootComponent);
	MaxBoundary->bHiddenInGame = true;

	// Load the sphere mesh for both boundaries
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere"));
	if (SphereMesh.Succeeded())
	{
		MinBoundary->SetStaticMesh(SphereMesh.Object);
		MaxBoundary->SetStaticMesh(SphereMesh.Object);
	}

	// Set the sphere mesh scale if needed
	MinBoundary->SetWorldScale3D(FVector(1.0f));  // Adjust scale if necessary
	MaxBoundary->SetWorldScale3D(FVector(1.0f));  // Adjust scale if necessary

	bIsCircle = false;
}

// Called when the game starts or when spawned
void APlayerBoundaries::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerBoundaries::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}