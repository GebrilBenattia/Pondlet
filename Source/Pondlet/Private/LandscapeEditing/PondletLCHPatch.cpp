// Fill out your copyright notice in the Description page of Project Settings.


#include "LandscapeEditing/PondletLCHPatch.h"

UPondletLCHPatch::UPondletLCHPatch()
{
	if (RadiusOverride >=0) {
		Radius = RadiusOverride;
	}
	if (FalloffOverride >= 0) {
		Falloff = FalloffOverride;
	}
}

void UPondletLCHPatch::SetRadiusOverride(float NewRadius)
{
	Radius = NewRadius;
}

void UPondletLCHPatch::SetFalloffOverride(float NewFalloff)
{
	Falloff = NewFalloff;
}
