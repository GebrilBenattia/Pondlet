// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LandscapeCircleHeightPatch.h"
#include "PondletLCHPatch.generated.h"

/**
 * 
 */
UCLASS()
class PONDLET_API UPondletLCHPatch : public ULandscapeCircleHeightPatch
{
	GENERATED_BODY()

public : 
	UPondletLCHPatch();

	void SetRadiusOverride(float NewRadius);

	void SetFalloffOverride(float NewFalloff);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pondlet", meta = (ExposeOnSpawn = true, Tooltip = "Override the LandscapeCircleHeightPatch Radius on spawn"))
	float RadiusOverride = -1.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pondlet", meta = (ExposeOnSpawn = true, Tooltip = "Override the LandscapeCircleHeightPatch Falloff on spawn"))
	float FalloffOverride = -1.0f;

	
};
