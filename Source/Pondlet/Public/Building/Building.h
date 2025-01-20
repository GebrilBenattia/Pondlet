// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.generated.h"

UCLASS()
class PONDLET_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuilding();



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ChangeBuildingMesh(UStaticMesh* Mesh);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlaySequence();

protected : 
	UPROPERTY(EditAnywhere)
	UStaticMesh* BuildingMesh;

	/*UPROPERTY(EditAnywhere)
	class UActorSequenceComponent* ActorSequence;

	UPROPERTY(EditAnywhere)
	class ACEClonerLayoutMesh * Layout;*/

};
