// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Building.h"
#include "ActorSequenceComponent.h"
#include "Cloner/Layouts/CEClonerMeshLayout.h"
#include "Cloner/CEClonerActor.h"
#include "Effector/CEEffectorActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABuilding::ABuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BuildingMesh");
	StaticMeshComponent->SetupAttachment(RootComponent);

	Tags.Add("PCGExclude");

}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();
	// Unreal builds crash when a Cloner is in the scene :)
	// TryInitLayout();
	// TryInitEffector();
	PlaySequence();
}

void ABuilding::OnConstruction(const FTransform& Transform)
{
	ActorSequenceComponent = GetComponentByClass<UActorSequenceComponent>();
}

void ABuilding::PlaySequence()
{
	if (ActorSequenceComponent) {
		ActorSequenceComponent->PlaySequence();
	}
}

void ABuilding::TryInitLayout()
{
	ACEClonerActor* Cloner = (ACEClonerActor*)UGameplayStatics::GetActorOfClass(GetWorld(), ClonerActorClass);
	
	if (Cloner) {
		MossLayout = (UCEClonerMeshLayout*)Cloner->GetActiveLayout();
		if (MossLayout) {
			MossLayout->SetCount(0);
			MossLayout->SetSampleActor(this);
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ABuilding needs a BP_Cloner"));
		return;
	}
}

void ABuilding::TryInitEffector()
{
	MossEffector = (ACEEffectorActor*)UGameplayStatics::GetActorOfClass(GetWorld(), ClonerEffectorClass);
	if(!MossEffector)
		UE_LOG(LogTemp, Error, TEXT("ABuilding needs a BP_Effector"));
}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (MossLayout) {
		int Instances = MossLayout->GetCount() + 1;
		Instances = FMath::Min(1000, Instances);
		MossLayout->SetCount(Instances);
	}
	else {
		// Unreal builds crash when a Cloner is in the scene :)
		// TryInitLayout();
	}

	if (MossEffector) {
		FVector EffectorLocation = MossEffector->GetActorLocation();
		EffectorLocation.Z += 0.1;
		UStaticMesh* BuildingMesh = StaticMeshComponent->GetStaticMesh();
		if (BuildingMesh) {
			float MaxZ = BuildingMesh->GetBounds().BoxExtent.Z;
			EffectorLocation.Z = FMath::Min(EffectorLocation.Z, MaxZ);
			MossEffector->SetActorLocation(EffectorLocation);
		}
	}
	else
	{
		// Unreal builds crash when a Cloner is in the scene :)
		//TryInitEffector();
	}


}


void ABuilding::ResetEffectorPosition()
{
	if (MossEffector) {
		MossEffector->SetActorLocation(GetActorLocation());
	}
}

void ABuilding::ResetMoss()
{
	ResetEffectorPosition();
	if (MossLayout) {
		MossLayout->SetCount(0);
	}
}

