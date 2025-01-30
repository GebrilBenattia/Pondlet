// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PCG/PCG_Actor.h"
#include "Metadata/PCGMetadata.h"
#include "PCG_Pond.generated.h"

/**
 *
 */
UCLASS()
class PONDLET_API APCG_Pond : public APCG_Actor
{
	GENERATED_BODY()


public:
	APCG_Pond();

	void OnConstruction(const FTransform& Transform) override;

	void BeginPlay() override;


	UFUNCTION(CallInEditor, Category = "DiscMeshes")
	void CoverPondByDiscs();

	UFUNCTION(CallInEditor, Category = "DiscMeshes")
	void RemovePondDiscs();

	float GetPondRadius() const { return MaxDistance; }

	void DigPhase();


protected:
	void PCGDataHandler(FPCGTaggedData Data) override;

	void RefreshPCG() override;

	UFUNCTION(CallInEditor, Category = "Spline Reset")
	void MakeCircleSpline();

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Spline Reset")
	void MakeEllipsisSpline();

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Spline Reset")
	void MakeLineSpline();

	UFUNCTION(CallInEditor, Category = "PCG")
	void EditorRegeneration();


private:

	void UpdateSphereRadius();


	void UpdateSplinePointsUsingPCGData(FPCGTaggedData Data);

	void DigUsingPCGData(FPCGTaggedData Data);

	void MakeBordersUsingPCGData(FPCGTaggedData Data);

	float PointToDepth(FPCGPoint Point, const FPCGMetadataAttribute<float>* Metadata)const;

	void CreateWaterBody();

	void AskForPicture();


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UPCGComponent* PCGSplineComponent;

	UPROPERTY(EditAnywhere)
	class USplineComponent* PondSpline;

	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere)
	class UStaticMesh* WaterPlane;

	UPROPERTY(EditAnywhere)
	class UMaterialInstance* WaterMaterial;

	UPROPERTY(EditAnywhere)
	class UInstancedStaticMeshComponent* FloorISMComponent;
	UPROPERTY(EditAnywhere)
	class UInstancedStaticMeshComponent* WallISMComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMesh* DiscMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector DiscSize = FVector(1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMesh* PondFloorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMesh* PondWallsMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UMaterialParameterCollection* LandscapeModifier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PondSettings", meta = (ExposeOnSpawn = "true"))
	int PondLength = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PondSettings", meta = (ExposeOnSpawn = "true"))
	int PondWidth = 700;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PondSettings", meta = (ExposeOnSpawn = "true"))
	int NumberOfRocks = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PondSettings", meta = (ExposeOnSpawn = "true"))
	int OutlineHeight = 50;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PondSettings", meta = (ExposeOnSpawn = "true"))
	int PondMaxDepth = 50;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PondSettings", meta = (ExposeOnSpawn = "true"))
	float PondDistanceToDepthMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PondSettings", meta = (ExposeOnSpawn = "true"))
	int SplineNumberOfPoints = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PondSettings", meta = (ExposeOnSpawn = "true"))
	float MaxTangentAngleOffset = 30;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PondSettings", meta = (ExposeOnSpawn = "true"))
	int MinTangentLength = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PondSettings", meta = (ExposeOnSpawn = "true"))
	int MaxTangentLength = 500;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PondSettings", meta = (ExposeOnSpawn = "true"))
	float LandscapeDiggingRadius = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PondSettings", meta = (ExposeOnSpawn = "true"))
	float LandscapeDiggingFalloff = 200;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PondSettings", meta = (ExposeOnSpawn = "true", ClampMin = "0"))
	float MinDistanceBetweenCenterAndSides = 200;
	UPROPERTY(BlueprintReadOnly)
	float MaxDistance;

private:

	UPROPERTY()
	TArray<FVector> DepthPointsLocation = TArray<FVector>();

	UPROPERTY()
	TArray<class UStaticMeshComponent*> DiscMeshComponents = TArray<class UStaticMeshComponent*>();


};
