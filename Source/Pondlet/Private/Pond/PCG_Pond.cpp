// Fill out your copyright notice in the Description page of Project Settings.


#include "Pond/PCG_Pond.h"
#include "PCGPoint.h"
#include "Data/PCGPointData.h"
#include "Metadata/PCGMetadata.h"
#include "Components/SplineComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/Pondlet_GameState.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "LandscapeEditing/LandscapePatchActor.h"

APCG_Pond::APCG_Pond()
{
	PondSpline = CreateDefaultSubobject<USplineComponent>("PondSpline");
	PondSpline->SetupAttachment(RootComponent);
	PondSpline->SetClosedLoop(true);
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(RootComponent);

	PCGSplineComponent = CreateDefaultSubobject<UPCGComponent>("PCGSplineComponent");
	PCGSplineComponent->GenerationTrigger = EPCGComponentGenerationTrigger::GenerateOnDemand;
	PCGSplineComponent->SetIsPartitioned(false);
	PCGSplineComponent->PostGenerateFunctionNames.Add(FName("PCGEndGraphCallback"));

	FloorISMComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>("FloorISMComponent");
	FloorISMComponent->SetupAttachment(RootComponent);
	WallISMComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>("WallISMComponent");
	WallISMComponent->SetupAttachment(RootComponent);
}

void APCG_Pond::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	PondSpline->SetRelativeLocation(FVector::ZeroVector);

	PondSpline->ClearSplinePoints();
	MakeEllipsisSpline();
	UpdateSphereRadius();
}

void APCG_Pond::BeginPlay()
{
	Super::BeginPlay();

	APondlet_GameState* GameState = (APondlet_GameState*)UGameplayStatics::GetGameState(this);
	if (GameState) {
		GameState->OnPictureTaken.AddDynamic(this, &APCG_Pond::RemovePondDiscs);
	}

	PondSpline->ClearSplinePoints();
	MakeEllipsisSpline();
	UpdateSphereRadius();
	RefreshPCG();

}

void APCG_Pond::CoverPondByDiscs()
{
	if (!DiscMesh)
		return;
	FTransform ActorTransform = GetActorTransform();
	for (const FVector Location : DepthPointsLocation) {
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(ActorTransform.InverseTransformPosition(Location) + FVector(0, 0, 50));
		SpawnTransform.SetScale3D(FVector((float)FMath::Max(MinDistanceBetweenCenterAndSides,75) / 50) * 1.1f);
		UStaticMeshComponent* DiscMeshComponent = (UStaticMeshComponent*)AddComponentByClass(UStaticMeshComponent::StaticClass(), false, SpawnTransform, false);
		DiscMeshComponent->SetStaticMesh(DiscMesh);
		DiscMeshComponents.Add(DiscMeshComponent);
	}
}

void APCG_Pond::RemovePondDiscs()
{
	for (UStaticMeshComponent* DiscMeshComponent : DiscMeshComponents) {
		DiscMeshComponent->DestroyComponent();
	}
	DiscMeshComponents.Empty();
}



void APCG_Pond::PCGDataHandler(FPCGTaggedData Data)
{
	Super::PCGDataHandler(Data);
	if (Data.Pin == "SplinePoints") {
		UpdateSplinePointsUsingPCGData(Data);
	}
	if (Data.Pin == "PondDigging") {
		DigUsingPCGData(Data);
	}
	if (Data.Pin == "PondOutline") {
		PCGMeshPointsHandler(Data);
		MakeBordersUsingPCGData(Data);
	}
}

void APCG_Pond::RefreshPCG()
{
	int Seed = FMath::Rand();
	PCGSplineComponent->Seed = Seed;
	PCGSplineComponent->SetIsPartitioned(false);
	PCGSplineComponent->NotifyPropertiesChangedFromBlueprint();
	PCGSplineComponent->Generate(false);
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

void APCG_Pond::MakeLineSpline()
{
	PondSpline->ClearSplinePoints();
	int SplinePoints = PondLength / 100;
	if (SplinePoints % 2 != 0) {
		SplinePoints++;
	}
	float StartX = -PondLength / 2;
	float XOffset = PondLength / (SplinePoints / 2);
	float YOffset = PondWidth / (SplinePoints / 2);
	for (int i = 0; i < SplinePoints; i++) {

		int YSign = (i % 2 == 0) ? 1 : -1;
		float X = i * XOffset;
		float Y = YOffset * YSign;
		FVector Offset = FVector(X, Y, 0);
		if (i == SplinePoints / 2) {
			PondSpline->SetRelativeLocation(-Offset);
		}
		PondSpline->AddSplinePoint(Offset, ESplineCoordinateSpace::Local);
	}

}

void APCG_Pond::MakeEllipsisSpline()
{
	PondSpline->ClearSplinePoints();
	int SplinePoints = PondLength / 25;
	for (int i = 0; i < SplinePoints; i++) {
		float AngleDeg = PI * 2 * ((float)i / SplinePoints);
		float X = PondLength * FMath::Cos(AngleDeg);
		float Y = PondWidth * FMath::Sin(AngleDeg);
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
	float CurrentMaxDist = 0;
	int SplinePoints = PondSpline->GetNumberOfSplinePoints();
	for (int i = 0; i < SplinePoints; i++) {
		FVector PointLocation = PondSpline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		float Distance = PointLocation.SquaredLength();
		CurrentMaxDist = FMath::Max(CurrentMaxDist, Distance);
	}
	MaxDistance = FMath::Sqrt(CurrentMaxDist);
	MaxDisplayRadius = MaxDistance;
	if (!bShouldAnimate) {
		DisplayRadius = MaxDistance;
	}
	UpdateDisplayRadius();
	SphereComponent->SetSphereRadius(MaxDistance);
}

void APCG_Pond::DigPhase()
{
	int Seed = FMath::Rand();
	PCGComponent->Seed = Seed;
	PCGComponent->SetIsPartitioned(false);
	PCGComponent->NotifyPropertiesChangedFromBlueprint();
	PCGComponent->Generate(false);
}

void APCG_Pond::UpdateSplinePointsUsingPCGData(FPCGTaggedData Data)
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

void APCG_Pond::DigUsingPCGData(FPCGTaggedData Data)
{
	DepthPointsLocation.Empty();
	UPCGPointData* PointData = (UPCGPointData*)Data.Data;
	const UPCGMetadata* Metadata = PointData->ConstMetadata();
	const FPCGMetadataAttribute<float>* DistanceMetadata = Metadata->GetConstTypedAttribute<float>("DistanceToSides");
	float MaxDepth = 10;
	for (FPCGPoint Point : PointData->GetPoints()) {
		float Depth = PointToDepth(Point, DistanceMetadata);
		MaxDepth = FMath::Max(Depth, MaxDepth);
		DepthPointsLocation.Add(Point.Transform.GetLocation());

		//FTransform Transform{
		//	Point.Transform.GetRotation(),
		//	 /*Point.Transform.GetLocation() +*/ FVector(0,0,-Depth),
		//	FVector((float)FMath::Max(MinDistanceBetweenCenterAndSides,75) / 50)
		//};
		//if (PondFloorMesh) {
		//	/*FloorISMComponent->SetStaticMesh(PondFloorMesh);
		//	FloorISMComponent->AddInstance(Transform, true);*/
		//}
	}

	FTransform Transform;
	Transform.SetLocation(FVector(0, 0, -MaxDepth));
	float Scale = MaxDistance / 50;
	Transform.SetScale3D(FVector(Scale, Scale, 1));


	UStaticMeshComponent* MeshComponent = (UStaticMeshComponent*)AddComponentByClass(UStaticMeshComponent::StaticClass(), false, Transform, false);
	if (MeshComponent) {
		MeshComponent->SetStaticMesh(PondFloorMesh);
	}

	CoverPondByDiscs();
	AskForPicture();
	CreateWaterBody();
}

void APCG_Pond::MakeBordersUsingPCGData(FPCGTaggedData Data)
{
	UPCGPointData* PointData = (UPCGPointData*)Data.Data;
	const UPCGMetadata* Metadata = PointData->ConstMetadata();
	const FPCGMetadataAttribute<FSoftObjectPath >* MetadataAttributePath = Metadata->GetConstTypedAttribute<FSoftObjectPath>("Mesh");
	const FPCGMetadataAttribute<double >* MetadataAttributeScale = Metadata->GetConstTypedAttribute<double>("Scale");
	for (FPCGPoint Point : PointData->GetPoints()) {
		FVector PointLocation = Point.Transform.GetLocation();
		FTransform RockTransform{
			Point.Transform.GetRotation(),
			Point.Transform.GetLocation() + FVector(0,0,OutlineHeight),
			Point.Transform.GetScale3D()
		};
		// Use Transform to spawn rocks (Use the foliage manager for that)
		FTransform WallTransform{
			Point.Transform.GetRotation() * FQuat::MakeFromEuler(FVector(0, 0, 180)),
			Point.Transform.GetLocation() + FVector(0,0,-PondMaxDepth),
			Point.Transform.GetScale3D() * 2.5f
		};
		if (PondWallsMesh) {
			WallISMComponent->SetStaticMesh(PondWallsMesh);
			WallISMComponent->AddInstance(WallTransform, true);
		}
	}
}

float APCG_Pond::PointToDepth(FPCGPoint Point, const FPCGMetadataAttribute<float>* Metadata) const
{
	float Value = Metadata->GetValueFromItemKey(Point.MetadataEntry);
	float Depth = FMath::Min(Value, PondMaxDepth) * PondDistanceToDepthMultiplier;
	return Depth;

}

void APCG_Pond::CreateWaterBody()
{
	FTransform WaterTransform;
	WaterTransform.SetLocation(FVector(0, 0, -10));

	if (WaterPlane) {
		UStaticMeshComponent* PlaneMesh = (UStaticMeshComponent*)AddComponentByClass(UStaticMeshComponent::StaticClass(), false, WaterTransform, false);
		if (PlaneMesh) {
			PlaneMesh->SetStaticMesh(WaterPlane);
			PlaneMesh->SetMaterial(0, WaterMaterial);
			PlaneMesh->SetRelativeScale3D(FVector(MaxDistance / 50, MaxDistance / 50, 1));
		}
	}
}

void APCG_Pond::AskForPicture()
{
	APondlet_GameState* GameState = (APondlet_GameState*)UGameplayStatics::GetGameState(this);
	if (GameState) {
		GameState->TakeMapScreen();
	}
}
