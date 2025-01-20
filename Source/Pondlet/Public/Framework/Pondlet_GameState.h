// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Pondlet_GameState.generated.h"


// Delegate signature
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTakePictureDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPictureTakenDelegate);

USTRUCT(BlueprintType)
struct FEcosystemSpawnParameters {
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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


public:
	UFUNCTION(BlueprintCallable)
	void ResetEcosystem(FEcosystemSpawnParameters SpawnParam);

	APondlet_GameState();

	void BeginPlay()override;

	UFUNCTION()
	void TakeMapScreen();

private:
	UFUNCTION()
	void SpawnStructure(class UStaticMesh* StructureMesh, FVector Location);

	UFUNCTION()
	void RandomPondGeneration();

	UFUNCTION()
	void PictureTakenBroadcast();

public:
	UPROPERTY(BlueprintAssignable)
	FOnTakePictureDelegate OnTakePicture;

	UPROPERTY(BlueprintAssignable)
	FOnPictureTakenDelegate OnPictureTaken;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABuilding> BuildingClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMapScreener> MapScreenerClass;

	UPROPERTY(EditAnywhere)
	FVector SimulationSize = FVector(4000, 4000, 200);


private:
	UPROPERTY(EditAnywhere)
	class ABuilding* Building;

	UPROPERTY(EditAnywhere)
	class AMapScreener* MapScreener;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class APCG_Pond> PCGPondClass;

};
