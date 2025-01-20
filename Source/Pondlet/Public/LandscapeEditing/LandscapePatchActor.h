// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LandscapePatchActor.generated.h"

UCLASS()
class PONDLET_API ALandscapePatchActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALandscapePatchActor();

	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform &Transform) override;

	void SetLandscapePatchRadius(float NewRadius);

	void SetLandscapePatchFalloff(float NewFalloff);
	
	void UpdatePatchValues();



public : 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pondlet", meta = (ExposeOnSpawn = true, Tooltip = "Sets the starting Radius on the LandscapePatchComponent"))
	float LandscapePatchRadius= -1.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pondlet", meta = (ExposeOnSpawn = true, Tooltip = "Sets the starting Falloff on the LandscapePatchComponent"))
	float LandscapePatchFalloff = -1.0f;

protected : 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pondlet")
	class USceneComponent* SceneComponent;
};
