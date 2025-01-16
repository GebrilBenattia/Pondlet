// Fill out your copyright notice in the Description page of Project Settings.


#include "Foliage/FoliageManager.h"
#include "Foliage/FoliageLogicComponent.h"
#include "AssetRegistry/IAssetRegistry.h"

// Sets default values
AFoliageManager::AFoliageManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFoliageManager::BeginPlay()
{
	Super::BeginPlay();

	TSet<UActorComponent*> Components = GetComponents();
	for (UActorComponent* Component : Components) {
		if(RootComponent != Component)
			Component->DestroyComponent();
	}

	Tags.Add("PCGExclude");

	MeshBuffer.Empty();
}

// Called every frame
void AFoliageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UStaticMeshComponent* AFoliageManager::SpawnFoliage(FTransform Transform, UStaticMesh* FoliageMesh)
{
	FTransform ActorTransform = GetActorTransform();
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(ActorTransform.InverseTransformPosition(Transform.GetLocation()));
	SpawnTransform.SetRotation(ActorTransform.InverseTransformRotation(Transform.GetRotation()));
	SpawnTransform.SetScale3D(Transform.GetScale3D());
	UStaticMeshComponent* MeshComponent = (UStaticMeshComponent*)AddComponentByClass(UStaticMeshComponent::StaticClass(), false, SpawnTransform, false);
	if (MeshComponent)
	{
		MeshComponent->SetStaticMesh(FoliageMesh);
		MeshComponent->ComponentTags.Add("Foliage");
		FTransform LogicTransform;
		UFoliageLogicComponent* FoliageLogicComponent = (UFoliageLogicComponent*)AddComponentByClass(UFoliageLogicComponent::StaticClass(), false, LogicTransform, false);
		if (FoliageLogicComponent)
		{
			FoliageLogicComponent->SetFoliageMesh(MeshComponent);
		}
		MeshComponent->SetVisibility(false);
		MeshComponent ->CastShadow = !ShadowlessMeshes.Contains(FoliageMesh);
	}
	return MeshComponent;
}

UStaticMesh* AFoliageManager::LoadMeshFromBuffer(FSoftObjectPath AssetPath)
{
	FString AssetName = AssetPath.GetAssetName();
	if (MeshBuffer.Contains(AssetName)) {
		return *MeshBuffer.Find(AssetName);
	}
	else {
		UStaticMesh* LoadMesh = (UStaticMesh*)AssetPath.TryLoad();
		if (LoadMesh) {
			MeshBuffer.Add(AssetName, LoadMesh);
			return LoadMesh;
		}
		return NULL;
	}
}

void AFoliageManager::ClearFoliage()
{
	TArray<UActorComponent*> Components;
	GetComponents(Components);
	for (UActorComponent * Component : Components) {
		if (Component->IsA(UFoliageLogicComponent::StaticClass()) || Component->IsA(UStaticMeshComponent::StaticClass())) {
			Component->DestroyComponent();
		}
	}
}

