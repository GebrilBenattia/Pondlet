// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LandscapeTexturePatch.h"
#include "PondletLTPatch.generated.h"

/**
 *
 */
UCLASS()
class PONDLET_API UPondletLTPatch : public ULandscapeTexturePatch
{
	GENERATED_BODY()


public : 
	UPondletLTPatch();
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pondlet", meta = (ExposeOnSpawn = true, Tooltip = "Override the LandscapeTexturePatch Falloff on spawn"))
	float FalloffOverride = -1.0f;
};
