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
	TryInitLayout();
	TryInitEffector();
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
	ACEClonerActor* Cloner = (ACEClonerActor*)UGameplayStatics::GetActorOfClass(GetWorld(), ACEClonerActor::StaticClass());
	if (Cloner) {
		MossLayout = (UCEClonerMeshLayout*)Cloner->GetActiveLayout();
		if (MossLayout) {
			MossLayout->SetCount(0);
			MossLayout->SetSampleActor(this);
		}

	}
}

void ABuilding::TryInitEffector()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACEEffectorActor::StaticClass(), OutActors);
	for (int i = 0; i < OutActors.Num(); i++) {
		TArray<UActorComponent*> Components = OutActors[i]->GetComponentsByTag(UActorComponent::StaticClass(), FName("MossEffector"));
		if (Components.Num() > 0) {
			MossEffector = (ACEEffectorActor*)OutActors[i];
			return;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("ABuilding needs a CEEffector with a component having the Tag \"Moss Effector\" or needs a BP_Effector"));
}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (MossLayout) {
		MossLayout->SetCount(MossLayout->GetCount() + 1);
	}
	else {
		TryInitLayout();
	}

	if (MossEffector) {
		FVector EffectorLocation = MossEffector->GetActorLocation();
		EffectorLocation.Z += 10;
		UStaticMesh* BuildingMesh = StaticMeshComponent->GetStaticMesh();
		if (BuildingMesh) {
			float MaxZ = BuildingMesh->GetBounds().BoxExtent.Z;
			EffectorLocation.Z = FMath::Min(EffectorLocation.Z, MaxZ) + GetActorLocation().Z;
			MossEffector->SetActorLocation(EffectorLocation);
		}
	}
	else
	{
		TryInitEffector();
	}


}


void ABuilding::ResetEffectorPosition()
{
	if (MossEffector) {
		MossEffector->SetActorLocation(GetActorLocation());
	}
}

