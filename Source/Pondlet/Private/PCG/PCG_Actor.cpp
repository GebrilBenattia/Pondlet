// Fill out your copyright notice in the Description page of Project Settings.


#include "PCG/PCG_Actor.h"
#include "PCGComponent.h"
#include "PCGGraph.h"
#include "PCGPoint.h"
#include "Data/PCGPointData.h"
#include "Foliage/FoliageManager.h"
#include "Kismet/GameplayStatics.h"
#include "Metadata/PCGMetadata.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Curves/CurveFloat.h"
#include "Components/TimelineComponent.h"


// Sets default values
APCG_Actor::APCG_Actor()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	PCGComponent = CreateDefaultSubobject<UPCGComponent>("PCGComponent");
	PCGComponent->GenerationTrigger = EPCGComponentGenerationTrigger::GenerateOnDemand;
	PCGComponent->bIsComponentPartitioned = false;
	PCGComponent->PostGenerateFunctionNames.Add(FName("PCGEndGraphCallback"));

	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(RootComponent);
	Meshes.Empty();

}

// Called when the game starts or when spawned
void APCG_Actor::BeginPlay()
{
	Super::BeginPlay();
	PCGComponent->Cleanup();

	int Seed = FMath::Rand();
	PCGComponent->Seed = Seed;

	FVector BoxExtent = BoxComponent->GetUnscaledBoxExtent();
	MaxDisplayRadius = FVector::Distance(FVector(0, 0, 0), BoxExtent);
	Meshes.Empty();


	FOnTimelineFloat onTimelineCallback;
	FOnTimelineEventStatic onTimelineFinishedCallback;
	if (FloatCurve != NULL)
	{
		SpawnTimeline = NewObject<UTimelineComponent>(this, FName("SpawnTimeline"));
		SpawnTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		this->BlueprintCreatedComponents.Add(SpawnTimeline);
		SpawnTimeline->SetNetAddressable();

		SpawnTimeline->SetPropertySetObject(this);
		SpawnTimeline->SetDirectionPropertyName(FName("TimelineDirection"));

		SpawnTimeline->SetLooping(false);
		SpawnTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
		SpawnTimeline->SetPlayRate(1 / AnimationDuration / 2);
		SpawnTimeline->SetPlaybackPosition(0.0f, false);

		//Add the float curve to the timeline and connect it to your timelines's interpolation function
		onTimelineCallback.BindUFunction(this, FName{ TEXT("TimelineCallback") });
		onTimelineFinishedCallback.BindUFunction(this, FName{ TEXT("TimelineFinishedCallback") });
		SpawnTimeline->AddInterpFloat(FloatCurve, onTimelineCallback, "CurveFloat");
		SpawnTimeline->SetTimelineFinishedFunc(onTimelineFinishedCallback);

		SpawnTimeline->RegisterComponent();

	}

	FoliageManager = (AFoliageManager*)UGameplayStatics::GetActorOfClass(GetWorld(), AFoliageManager::StaticClass());

	if (SpawnTimeline != NULL && bShouldAnimate) {
		RefreshPCG();
		PlayTimeline();
	}
	else {
		DisplayRadius = MaxDisplayRadius;
	}
}

void APCG_Actor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UpdateVolumeSize(VolumeSize);
	ActorPositionPCG = GetActorLocation();
}

void APCG_Actor::UpdateDisplayRadius()
{
	for (UStaticMeshComponent* Mesh : Meshes) {
		if (!Mesh) return;
		FVector MeshLocation = Mesh->GetComponentLocation();
		FVector ActorLocation = GetActorLocation();
		float Distance = FVector::Distance(MeshLocation, ActorLocation);
		if (Distance < DisplayRadius) {
			Mesh->SetVisibility(true);
		}
		else {
			Mesh->SetVisibility(false);
		}
	}
}

void APCG_Actor::UpdateVolumeSize(FVector NewVolume)
{
	BoxComponent->SetBoxExtent(NewVolume);
	BoxComponent->SetRelativeLocation(FVector(0, 0, BoxComponent->Bounds.BoxExtent.Z));
}

void APCG_Actor::PCGEndGraphCallback(FPCGDataCollection Output)
{
	if (!FoliageManager) {
		FoliageManager = (AFoliageManager*)UGameplayStatics::GetActorOfClass(GetWorld(), AFoliageManager::StaticClass());
	}
	if (!FoliageManager) return;
	for (const FPCGTaggedData Data : Output.TaggedData) {
		UE_LOG(LogTemp, Warning, TEXT("Pin : %s"), *Data.Pin.ToString());
		PCGDataHandler(Data);
	}
	UpdateDisplayRadius();
}

void APCG_Actor::SpawnFoliage(UStaticMesh* Mesh, FTransform Transform)
{
	UStaticMeshComponent* MeshComponent = FoliageManager->SpawnFoliage(Transform, Mesh);
	if (MeshComponent) {
		Meshes.Add(MeshComponent);
	}
}

void APCG_Actor::PCGDataHandler(FPCGTaggedData Data)
{
	if (Data.Pin == "MeshPoints") {
		PCGMeshPointsHandler(Data);
	}
}

void APCG_Actor::PCGMeshPointsHandler(FPCGTaggedData Data)
{
	const UPCGPointData* PointData = (UPCGPointData*)Data.Data;

	const UPCGMetadata* Metadata = PointData->Metadata;
	const FPCGMetadataAttribute<FSoftObjectPath >* MetadataAttributePath = Metadata->GetConstTypedAttribute<FSoftObjectPath>("Mesh");
	const FPCGMetadataAttribute<double >* MetadataAttributeScale = Metadata->GetConstTypedAttribute<double>("Scale");
	for (const FPCGPoint Point : PointData->GetPoints()) {

		FSoftObjectPath MeshPath = MetadataAttributePath->GetValueFromItemKey(Point.MetadataEntry);
		double Scale = MetadataAttributeScale->GetValueFromItemKey(Point.MetadataEntry);
		UStaticMesh* Mesh = FoliageManager->LoadMeshFromBuffer(MeshPath);
		FTransform Transform{ Point.Transform.GetRotation().GetNormalized(),Point.Transform.GetLocation(),  FVector(Scale) * Point.Transform.GetScale3D()};
		if (Mesh) {
			SpawnFoliage(Mesh, Transform);
		}
	}
}


// Called every frame
void APCG_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (SpawnTimeline != NULL)
	{
		SpawnTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
	}
	ActorPositionPCG = GetActorLocation();

}

void APCG_Actor::SetGraph(UPCGGraph* Graph)
{
	PCGComponent->SetGraph(Cast<UPCGGraphInterface>(Graph));
}

void APCG_Actor::TimelineCallback(float val)
{
	DisplayRadius = FMath::Lerp(0, MaxDisplayRadius, val);
	UpdateDisplayRadius();
}

void APCG_Actor::TimelineFinishedCallback()
{
}

void APCG_Actor::PlayTimeline()
{
	if (SpawnTimeline != NULL)
	{
		SpawnTimeline->PlayFromStart();
	}
}

void APCG_Actor::RefreshPCG()
{
	PCGComponent->NotifyPropertiesChangedFromBlueprint();
	PCGComponent->Generate(false);
}
