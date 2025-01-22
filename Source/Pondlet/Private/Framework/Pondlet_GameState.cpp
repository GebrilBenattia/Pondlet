// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/Pondlet_Gamestate.h"
#include "Kismet/GameplayStatics.h"
#include "PCG/PCG_Actor.h"
#include "PCGGraph.h"
#include "Foliage/FoliageManager.h"
#include "Building/Building.h"
#include "Engine/StaticMeshActor.h"
#include "LandscapeEditing/MapScreener.h"
#include "Pond/PCG_Pond.h"
#include "Building/BuildingSpawner.h"



APondlet_GameState::APondlet_GameState()
{
	static ConstructorHelpers::FClassFinder<ABuilding> BuildingClassFinder(TEXT("/Game/Blueprints/Building/BP_House.BP_House_C"));
	if (BuildingClassFinder.Succeeded()) {
		BuildingClass = BuildingClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<AMapScreener> MapScreenClassFinder(TEXT("/Game/Blueprints/LandscapeEditing/BP_MapScreener.BP_MapScreener_C"));
	if (MapScreenClassFinder.Succeeded()) {
		MapScreenerClass = MapScreenClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<APCG_Pond> PondClassFinder(TEXT("/Game/Blueprints/PCG/BP_Pond.BP_Pond_C"));
	if (PondClassFinder.Succeeded()) {
		PCGPondClass = PondClassFinder.Class;
	}



}

void APondlet_GameState::BeginPlay()
{
	Super::BeginPlay();
	FTransform Transform;
	FActorSpawnParameters SpawnParams;
	if (MapScreenerClass) {
		MapScreener = GetWorld()->SpawnActor<AMapScreener>(MapScreenerClass, Transform, SpawnParams);
		// Bind to the delegate
		if (MapScreener) {
			MapScreener->OnPictureTaken.BindUFunction(this, "PictureTakenBroadcast");
		}
	}

	if (BuildingSpawnerClass) {
		FTransform SpawnerTransform;
		Transform.SetLocation(BuildingSpawnPos);
		BuildingSpawner = GetWorld()->SpawnActor<ABuildingSpawner>(BuildingSpawnerClass, Transform, SpawnParams);
	}
}

void APondlet_GameState::ResetEcosystem(FEcosystemSpawnParameters SpawnParam)
{
	// Kill all the species

	// Remove all the PCG areas
	TArray<AActor*> PCGActors;
	UGameplayStatics::GetAllActorsOfClass(this, APCG_Actor::StaticClass(), PCGActors);
	for (AActor* PCGActor : PCGActors) {
		PCGActor->Destroy();
	}

	// Remove all the foliage
	AFoliageManager* FoliageManager = (AFoliageManager*)UGameplayStatics::GetActorOfClass(this, AFoliageManager::StaticClass());
	if (FoliageManager) {
		FoliageManager->ClearFoliage();
	}
	// Spawn the new structure according to the parameters
	SpawnStructure(SpawnParam.BuildingClass, SpawnParam.Location);

	// Spawn the Pond
	RandomPondGeneration();

	// Spawn the starting foliage
	if (SpawnParam.PCGActorClass == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("PCG Actor Class is null"));
		return;
	}
	FActorSpawnParameters ActorSpawnParams;
	FTransform Transform;
	Transform.SetLocation(SpawnParam.Location);
	APCG_Actor* PCGActor = GetWorld()->SpawnActor<APCG_Actor>(SpawnParam.PCGActorClass, Transform, ActorSpawnParams);
	if (SpawnParam.PCGGraph == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("PCG Graph is null"));
		return;
	}
	PCGActor->SetGraph(SpawnParam.PCGGraph);

	// 
	// Set the other settings such as Weather/Season/Daytime


}





void APondlet_GameState::SpawnStructure(TSubclassOf<class ABuilding>  NewBuildingClass, FVector Location)
{
	if (BuildingSpawner) {
		BuildingSpawner->ChangeBuilding(NewBuildingClass);

	}
	ABuilding* Building = (ABuilding * )UGameplayStatics::GetActorOfClass(GetWorld(),ABuilding::StaticClass());
	if (Building) {
		Building->ResetMoss();
	}
	/*Building->ChangeBuildingMesh(StructureMesh);
	Building->SetActorLocation(Location);
	Building->Tags.Add("PCGExclude");*/
}

void APondlet_GameState::RandomPondGeneration()
{
	if (!PCGPondClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("PCGPondClass is null"));
		return;
	}
	FTransform Transform;
	FActorSpawnParameters SpawnParams;
	APCG_Pond* Pond = GetWorld()->SpawnActor<APCG_Pond>(PCGPondClass, Transform, SpawnParams);
	if (!Pond)
		return;
	TArray<FVector2D> LandscapePartsIndex = { FVector2D(-1,-1),FVector2D(-1,0),FVector2D(0,-1),FVector2D(0,0) };
	bool bCorrectPosition = false;
	float PartWidth = SimulationSize.X / 2;
	float PartLength = SimulationSize.Y / 2;
	float PondRadius = Pond->GetPondRadius();
	while (!bCorrectPosition && LandscapePartsIndex.Num() > 0) {
		int Index = FMath::RandRange(0, LandscapePartsIndex.Num() - 1);
		FVector2D PartIndex = LandscapePartsIndex[Index];
		float XLowBorder = PartWidth * PartIndex.X + PondRadius;
		float XHighBorder = PartWidth * (PartIndex.X + 1) - PondRadius;
		float YLowBorder = PartLength * PartIndex.Y + PondRadius;
		float YHighBorder = PartLength * (PartIndex.Y + 1) - PondRadius;
		int X = FMath::RandRange((int)XLowBorder, (int)XHighBorder);
		int Y = FMath::RandRange((int)YLowBorder, (int)YHighBorder);
		FVector Location = FVector(X, Y, 100);
		Pond->SetActorLocation(Location);
		TArray<AActor*> OverlappingActors;
		Pond->GetOverlappingActors(OverlappingActors);
		bool bIsOverlapping = false;
		for (AActor* OverlappingActor : OverlappingActors)
		{
			if (OverlappingActor->ActorHasTag("PCGExclude")) {
				LandscapePartsIndex.RemoveAt(Index);
				bIsOverlapping = true;
				break;
			}
		}
		bCorrectPosition = !bIsOverlapping;
	}

	if (bCorrectPosition) {
		Pond->DigPhase();
	}


}

void APondlet_GameState::TakeMapScreen()
{
	OnTakePicture.Broadcast();
	MapScreener->TakeScreenShot();
}

void APondlet_GameState::PictureTakenBroadcast()
{
	OnPictureTaken.Broadcast();
}
