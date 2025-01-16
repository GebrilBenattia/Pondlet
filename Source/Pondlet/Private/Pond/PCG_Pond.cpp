// Fill out your copyright notice in the Description page of Project Settings.


#include "Pond/PCG_Pond.h"
#include "PCGPoint.h"
#include "Data/PCGPointData.h"
#include "Components/SplineComponent.h"
#include "Components/SphereComponent.h"

APCG_Pond::APCG_Pond()
{
	PondSpline = CreateDefaultSubobject<USplineComponent>("PondSpline");
	PondSpline->SetupAttachment(RootComponent);
	PondSpline->SetClosedLoop(true);
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(RootComponent);
}

void APCG_Pond::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	/*PondSpline->ClearSplinePoints();
	MakeCircleSpline();
	UpdateSphereRadius();*/
}

void APCG_Pond::PCGDataHandler(FPCGTaggedData Data)
{
	Super::PCGDataHandler(Data);
	if (Data.Pin == "SplinePoints") {
		UpdateSplinePointsFromPCG(Data);
	}
}

void APCG_Pond::MakeCircleSpline()
{
	PondSpline->ClearSplinePoints();
	int SplinePoints = PondLength / 25;
	for (int i = 0; i < SplinePoints; i++) {
		float AngleDeg = PI * 2 * ((float)i / SplinePoints);
		float X = PondLength * FMath::Cos(AngleDeg);
		float Y = PondLength * FMath::Sin(AngleDeg);
		PondSpline->AddSplinePoint(FVector(X, Y, 0), ESplineCoordinateSpace::Local);
	}
}

void APCG_Pond::EditorRegeneration()
{
	bool bWasActivated = PCGComponent->bActivated;
	PCGComponent->Cleanup();
	PCGComponent->bActivated = true;
	int Seed = FMath::Rand();
	PCGComponent->Seed = Seed;
	PCGComponent->Generate();
	PCGComponent->bActivated = bWasActivated;
}

void APCG_Pond::UpdateSphereRadius()
{
	float MaxDistance = 0;
	int SplinePoints = PondSpline->GetNumberOfSplinePoints();
	for (int i = 0; i < SplinePoints; i++) {
		FVector PointLocation = PondSpline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		float Distance = PointLocation.SquaredLength();
		MaxDistance = FMath::Max(MaxDistance, Distance);
	}
	SphereComponent->SetSphereRadius(FMath::Sqrt(MaxDistance));
}

void APCG_Pond::UpdateSplinePointsFromPCG(FPCGTaggedData Data)
{
	PondSpline->ClearSplinePoints();
	const UPCGPointData* PointsData = (UPCGPointData*)Data.Data;
	for (const FPCGPoint Point : PointsData->GetPoints()) {
		int TangentLength = FMath::RandRange(MinTangentLength, MaxTangentLength);
		PondSpline->AddSplinePoint(Point.Transform.GetLocation(), ESplineCoordinateSpace::World);
		FVector Tangent = PondSpline->GetTangentAtSplinePoint(Point.MetadataEntry, ESplineCoordinateSpace::World).GetSafeNormal();
		int RotAngle = FMath::RandRange(-MaxTangentAngleOffset, MaxTangentAngleOffset);
		Tangent = Tangent.RotateAngleAxis(RotAngle, FVector::ZAxisVector);
		Tangent *= TangentLength;
		PondSpline->SetTangentAtSplinePoint(Point.MetadataEntry, Tangent, ESplineCoordinateSpace::World);
	}
	UpdateSphereRadius();
}
