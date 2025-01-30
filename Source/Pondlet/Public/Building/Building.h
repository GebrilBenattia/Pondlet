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

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void ResetEffectorPosition();

	void ResetMoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	void PlaySequence();

private:
	void TryInitLayout();

	void TryInitEffector();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, meta = (Tooltip = "You NEED to add an ActorSequencer in the blueprint"))
	class UActorSequenceComponent* ActorSequenceComponent;

	UPROPERTY(EditAnywhere)
	class UCEClonerMeshLayout* MossLayout;

	UPROPERTY(EditAnywhere)
	class ACEEffectorActor* MossEffector;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACEEffectorActor> ClonerEffectorClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACEClonerActor> ClonerActorClass;

};
