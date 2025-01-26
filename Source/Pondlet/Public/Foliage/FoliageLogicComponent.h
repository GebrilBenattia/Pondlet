// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FoliageLogicComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PONDLET_API UFoliageLogicComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFoliageLogicComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public : 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Oldness = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeToGrow = 10;

	void SetFoliageMesh(UStaticMeshComponent* Mesh);
	
	void AgeFoliage(float Time);

	UPROPERTY(EditAnywhere)
	FVector MinScale = FVector(0.2);

	UPROPERTY(EditAnywhere)
	FVector MaxScale = FVector(1);


protected : 
	UPROPERTY()
	UStaticMeshComponent* FoliageMesh;

	FVector FinalScale;
};
