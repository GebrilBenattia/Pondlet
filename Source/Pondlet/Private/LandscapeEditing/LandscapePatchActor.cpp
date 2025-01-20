// Fill out your copyright notice in the Description page of Project Settings.


#include "LandscapeEditing/LandscapePatchActor.h"

// Sets default values
ALandscapePatchActor::ALandscapePatchActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

}

void ALandscapePatchActor::BeginPlay()
{
	Super::BeginPlay();
	UpdatePatchValues();
}

void ALandscapePatchActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UpdatePatchValues();
}

void ALandscapePatchActor::UpdatePatchValues() {
	
}

void ALandscapePatchActor::SetLandscapePatchRadius(float NewRadius)
{
	LandscapePatchRadius = NewRadius;
}

void ALandscapePatchActor::SetLandscapePatchFalloff(float NewFalloff)
{
	LandscapePatchFalloff = NewFalloff;
}


