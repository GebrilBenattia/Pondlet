// Fill out your copyright notice in the Description page of Project Settings.


#include "PCG/PCG_FlowerField.h"
#include "PCGPoint.h"
#include "Data/PCGPointData.h"
#include "Components/BoxComponent.h"
#include "Foliage/FoliageManager.h"

APCG_FlowerField::APCG_FlowerField()
{
	PCGSpreadingComponent = CreateDefaultSubobject<UPCGComponent>("PCGSpreadingComponent");
	PCGSpreadingComponent->GenerationTrigger = EPCGComponentGenerationTrigger::GenerateOnDemand;
}

void APCG_FlowerField::BeginPlay()
{
	Super::BeginPlay();
	int AverageChildDistance = (MaxDistanceChild + MinDistanceChild) / 2;
	float Size = BoxComponent->GetLocalBounds().SphereRadius - 200;
	UE_LOG(LogTemp, Warning, TEXT("Size : %f"), Size);
	SpreadingsLeft = FMath::FloorToInt(Size / AverageChildDistance);
}

void APCG_FlowerField::TimelineFinishedCallback()
{
	Super::TimelineFinishedCallback();


}

void APCG_FlowerField::PCGDataHandler(FPCGTaggedData Data)
{
	Super::PCGDataHandler(Data);
	if (Data.Pin == "NextFlowerPoints")
	{
		PCGNextFlowerPoints(Data);
	}
}

void APCG_FlowerField::PCGNextFlowerPoints(FPCGTaggedData Data)
{
	FlowerSpreadParameters.Empty();
	const UPCGPointData* PointsData = (UPCGPointData*)Data.Data;


	const UPCGMetadata* Metadata = PointsData->Metadata;
	const FPCGMetadataAttribute<FSoftObjectPath >* MetadataAttributePath = Metadata->GetConstTypedAttribute<FSoftObjectPath>("Mesh");
	const FPCGMetadataAttribute<double >* MetadataAttributeScale = Metadata->GetConstTypedAttribute<double>("Scale");
	SpreadingPoints = PointsData->GetPoints();
	for (const FPCGPoint Point : PointsData->GetPoints()) {

		FSoftObjectPath MeshPath = MetadataAttributePath->GetValueFromItemKey(Point.MetadataEntry);
		double Scale = MetadataAttributeScale->GetValueFromItemKey(Point.MetadataEntry);
		UStaticMesh* Mesh = FoliageManager->LoadMeshFromBuffer(MeshPath);
		FTransform Transform{ Point.Transform.GetRotation(),Point.Transform.GetLocation(),  FVector(Scale) * Point.Transform.GetScale3D() };
		if (Mesh) {
			int Angle = FMath::RandRange(-MaxAngleOffsetFromDirection, MaxAngleOffsetFromDirection);
			int DistanceChild = FMath::RandRange(MinDistanceChild, MaxDistanceChild);
			FVector Direction = (Point.Transform.GetLocation() - GetActorLocation()).GetSafeNormal();
			FlowerSpreadParameters.Add(FFlowerSpreadParameters{ Direction,(float)Angle,(float)DistanceChild,Mesh });
		}
	}
	if (SpreadingsLeft > 0)
	{
		SpreadingsLeft--;
		GetWorld()->GetTimerManager().SetTimer(SpreadingTimerHandle, this, &APCG_FlowerField::PCGSpreadFlowers, TimeBetweenSpreadings, false);
	}
}

void APCG_FlowerField::PCGSpreadFlowers()
{
	int Seed = FMath::Rand();
	PCGSpreadingComponent->Seed = Seed;
	PCGSpreadingComponent->NotifyPropertiesChangedFromBlueprint();
	PCGSpreadingComponent->GenerateLocal(false);
}
