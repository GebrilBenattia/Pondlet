// Fill out your copyright notice in the Description page of Project Settings.


#include "Foliage/FoliageLogicComponent.h"

// Sets default values for this component's properties
UFoliageLogicComponent::UFoliageLogicComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFoliageLogicComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...

}


// Called every frame
void UFoliageLogicComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	AgeFoliage(DeltaTime);
}

void UFoliageLogicComponent::SetFoliageMesh(UStaticMeshComponent* Mesh)
{
	FoliageMesh = Mesh;
	FinalScale = Mesh->GetRelativeScale3D();
}

void UFoliageLogicComponent::AgeFoliage(float Time)
{
	Oldness += Time;
	if (FoliageMesh) {
		float t = FMath::Clamp(Oldness / TimeToGrow,0,1);
		FVector NextScale = FMath::Lerp(FVector(0.2), FinalScale,t);
		FoliageMesh->SetWorldScale3D(NextScale);
	}
}

