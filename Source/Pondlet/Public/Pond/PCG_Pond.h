// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PCG/PCG_Actor.h"
#include "PCG_Pond.generated.h"

/**
 * 
 */
UCLASS()
class PONDLET_API APCG_Pond : public APCG_Actor
{
	GENERATED_BODY()


public : 
	APCG_Pond();

	void OnConstruction(const FTransform& Transform) override;

protected : 
	void PCGDataHandler(FPCGTaggedData Data) override;

	UFUNCTION(CallInEditor,Category="Spline Reset")
	void MakeCircleSpline();

	UFUNCTION(CallInEditor,BlueprintCallable,Category = "Spline Reset")
	void MakeEllipsisSpline();

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Spline Reset")
	void MakeLineSpline();

	UFUNCTION(CallInEditor, Category = "PCG")
	void EditorRegeneration();

private : 

	void UpdateSphereRadius();

	void UpdateSplinePointsUsingPCGData(FPCGTaggedData Data);

	void DigUsingPCGData(FPCGTaggedData Data);

	void MakeBordersUsingPCGData(FPCGTaggedData Data);

protected :
	UPROPERTY(EditAnywhere)
	class USplineComponent* PondSpline;

	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	int PondLength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	int PondWidth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	int SplineNumberOfPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	float MaxTangentAngleOffset = 30;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	int MinTangentLength = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	int MaxTangentLength = 500;
	

};
