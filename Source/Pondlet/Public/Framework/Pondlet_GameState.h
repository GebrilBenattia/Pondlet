// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Pondlet_GameState.generated.h"


USTRUCT(BlueprintType)
struct FEcosystemSpawnParameters {
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class APCG_Actor> PCGActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPCGGraph* PCGGraph;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* Structure;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;
};
/**
 *
 */
UCLASS()
class PONDLET_API APondlet_GameState : public AGameStateBase
{
	GENERATED_BODY()


public : 
	UFUNCTION(BlueprintCallable)
	void ResetEcosystem(FEcosystemSpawnParameters SpawnParam);

	APondlet_GameState();

private : 
	UFUNCTION()
	void SpawnStructure(class UStaticMesh* StructureMesh,FVector Location);

protected : 
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABuilding> BuildingClass;

private : 
	UPROPERTY(EditAnywhere)
	class AStaticMeshActor* Structure;

};
