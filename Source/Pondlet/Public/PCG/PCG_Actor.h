// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "PCGComponent.h"
#include "PCG_Actor.generated.h"

UCLASS()
class APCG_Actor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APCG_Actor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void SetGraph(class UPCGGraph* Graph);




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	void UpdateDisplayRadius();

	UFUNCTION()
	virtual void TimelineFinishedCallback();

	UFUNCTION()
	virtual void PCGDataHandler(FPCGTaggedData Data);

	UFUNCTION()
	void PCGMeshPointsHandler(FPCGTaggedData Data);

	UFUNCTION(CallInEditor)
	virtual void PCGEndGraphCallback(FPCGDataCollection Output);

	UFUNCTION()
	virtual void RefreshPCG();

private:
	UFUNCTION()
	void TimelineCallback(float val);

	void PlayTimeline();

	void UpdateVolumeSize(FVector NewVolume);


	UFUNCTION()
	void SpawnFoliage(UStaticMesh* Mesh, FTransform Transform);




public:

	UPROPERTY(EditAnywhere, Category = "PCGAttributes", BlueprintReadOnly, meta = (ExposeOnSpawn = "true", ToolTip = "Tells the size needed for each point you want to generate"))
	FVector PointExtent = FVector(5,5, 1);

	UPROPERTY(EditAnywhere, Category = "PCGAttributes", BlueprintReadOnly, meta = (ExposeOnSpawn = "true", ToolTip = "Tells how many points you want to generate in a squared meter"))
	float PointPerSquaredMeter = 10.0f;

	UPROPERTY(EditAnywhere, Category = "PCGAttributes", BlueprintReadOnly, meta = (ExposeOnSpawn = "true", ToolTip = "Tells how much the points will be on a grid (The more the value the more apparent is the grid)"))
	float Looseness = 1.0f;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	FVector ActorPositionPCG;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UPCGComponent* PCGComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, meta = (ToolTip = "Only x = 0 to x = 1 in the curve will matter"))
	UCurveFloat* FloatCurve;

	UPROPERTY()
	float CurveFloat;
	UPROPERTY(EditAnywhere)
	float AnimationDuration = 2.0f;

	UPROPERTY(EditAnywhere, meta = (ToolTip = "Choose whether the spawn of the PCG elements is progressive or if all the elements shall appear at the same time"))
	bool bShouldAnimate = true;


	UPROPERTY(EditAnywhere)
	FVector VolumeSize = FVector(100, 100, 100);

	UPROPERTY()
	class AFoliageManager* FoliageManager;

	UPROPERTY()
	float DisplayRadius = 0.0f;

	UPROPERTY()
	float MaxDisplayRadius = 100.0f;


private:
	UPROPERTY()
	UTimelineComponent* SpawnTimeline;

	UPROPERTY()
	TEnumAsByte<ETimelineDirection::Type> TimelineDirection;


	


	UPROPERTY()
	TArray<class UStaticMeshComponent*> Meshes;


};
