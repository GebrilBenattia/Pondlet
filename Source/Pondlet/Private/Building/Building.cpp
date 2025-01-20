// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Building.h"
//#include "ActorSequenceComponent.h"
//#include "CEClonerLayoutMesh.h"

// Sets default values
ABuilding::ABuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//ActorSequence = CreateDefaultSubobject<UActorSequenceComponent>(FName("ActorSequence"));

	

}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();

}

void ABuilding::PlaySequence()
{

}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuilding::ChangeBuildingMesh(UStaticMesh* Mesh)
{
	if (Mesh) {
		BuildingMesh = Mesh;
		PlaySequence();
	}
}

