// Fill out your copyright notice in the Description page of Project Settings.


#include "LandscapeEditing/PondletLTPatch.h"

UPondletLTPatch::UPondletLTPatch()
{
	if (FalloffOverride > 0.0f)
	{
		//Super::Radius = RadiusOverride;
		SetFalloff(FalloffOverride);
	}
}
