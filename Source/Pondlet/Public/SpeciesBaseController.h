// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SpeciesBaseController.generated.h"

/**
 * 
 */
UCLASS()
class PONDLET_API ASpeciesBaseController : public AAIController
{
	GENERATED_BODY()
	
public:

	ASpeciesBaseController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void BeginPlay() override;
};
