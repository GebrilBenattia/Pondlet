// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PCG/PCG_Actor.h"
#include "PCG_FlowerField.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FFlowerSpreadParameters {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	FVector Direction;

	UPROPERTY(BlueprintReadOnly,meta = (ExposeOnSpawn = "true"))
	float Angle;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	float DistanceChild;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	class UStaticMesh * Mesh;

};
UCLASS()
class PONDLET_API APCG_FlowerField : public APCG_Actor
{
	GENERATED_BODY()

public:
	APCG_FlowerField();
	
protected : 
	void BeginPlay() override;

	void TimelineFinishedCallback() override;

	void PCGDataHandler(FPCGTaggedData Data) override;

	void PCGNextFlowerPoints(FPCGTaggedData Data);


	void PCGSpreadFlowers();

public : 
	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TArray<FPCGPoint> SpreadingPoints;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TArray<int> AngleOffsets; // In Degrees

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TArray<float> ChildDistances;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TArray<FFlowerSpreadParameters> FlowerSpreadParameters;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	FFlowerSpreadParameters FlowerSpreadParameter;

	bool FirstBatch = true;

protected : 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UPCGComponent* PCGSpreadingComponent;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int SpreadingsLeft = 3;


	UPROPERTY(EditAnywhere)
	int MinDistanceChild = 100;

	UPROPERTY(EditAnywhere)
	int MaxDistanceChild = 100;

	UPROPERTY(EditAnywhere)
	int MaxAngleOffsetFromDirection = 90;

	UPROPERTY(EditAnywhere)
	float TimeBetweenSpreadings = 2.5f;

private : 
	UPROPERTY()
	FTimerHandle SpreadingTimerHandle;

	float SpreadingDistance = 100.0f;
};
