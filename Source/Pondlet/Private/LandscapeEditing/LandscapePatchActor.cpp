// Fill out your copyright notice in the Description page of Project Settings.


#include "LandscapeEditing/LandscapePatchActor.h"
#include "LandscapeEditing/PondletLCHPatch.h"

// Sets default values
ALandscapePatchActor::ALandscapePatchActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	LandscapeCircleHeightPatchComponent = CreateDefaultSubobject<UPondletLCHPatch>(TEXT("LandscapeCircleHeightPatchComponent"));
	LandscapeCircleHeightPatchComponent->SetupAttachment(RootComponent);
}

void ALandscapePatchActor::BeginPlay()
{
	
}

void ALandscapePatchActor::OnConstruction(const FTransform& Transform)
{
	if (LandscapeCircleHeightPatchComponent)
	{
		if (LandscapePatchRadius >= 0.0f)
		{
			LandscapeCircleHeightPatchComponent->SetRadiusOverride(LandscapePatchRadius);
		}
		if (LandscapePatchFalloff >= 0.0f)
		{
			LandscapeCircleHeightPatchComponent->SetFalloffOverride(LandscapePatchFalloff);
		}
	}
}


