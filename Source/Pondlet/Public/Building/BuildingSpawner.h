// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingSpawner.generated.h"

UCLASS()
class PONDLET_API ABuildingSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingSpawner();


	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ChangeBuilding(TSubclassOf<class ABuilding> NewBuildingClass);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
