// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/Pondlet_Gamestate.h"
#include "Kismet/GameplayStatics.h"
#include "PCG/PCG_Actor.h"
#include "PCGGraph.h"
#include "Foliage/FoliageManager.h"
#include "Building/Building.h"
#include "Engine/StaticMeshActor.h"
//#include ""



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

	// Remove the structure
	if (Structure)
		Structure->Destroy();
	// Spawn the new structure according to the parameters
	SpawnStructure(SpawnParam.Structure, SpawnParam.Location);

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

APondlet_GameState::APondlet_GameState()
{
	static ConstructorHelpers::FClassFinder<ABuilding> BuildingClassFinder(TEXT("/Game/Blueprints/Building/BP_House.BP_House_C"));
	if (BuildingClassFinder.Succeeded()) {
		BuildingClass = BuildingClassFinder.Class;
	}
}

void APondlet_GameState::SpawnStructure(UStaticMesh* StructureMesh, FVector Location)
{
	if (!StructureMesh) {
		UE_LOG(LogTemp, Warning, TEXT("Structure is null"));
		return;
	}
	ABuilding* Building = (ABuilding*)UGameplayStatics::GetActorOfClass(this, ABuilding::StaticClass());
	if (!Building) {
		FTransform Transform;
		FActorSpawnParameters spawnParams;

		Transform.SetLocation(Location);
		Building = GetWorld()->SpawnActor<ABuilding>(BuildingClass, Transform, spawnParams);
	}

	if (BuildingClass) {
		UE_LOG(LogTemp,Display,TEXT("Class found"))
	}

	Building->ChangeBuildingMesh(StructureMesh);
	Building->SetActorLocation(Location);
	Building->Tags.Add("PCGExclude");
}
