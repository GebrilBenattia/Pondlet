// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapScreener.generated.h"


DECLARE_DYNAMIC_DELEGATE(FOnPictureTakenSignature);
UCLASS()
class PONDLET_API AMapScreener : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapScreener();

	void TakeScreenShot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public : 
	UPROPERTY()
	FOnPictureTakenSignature OnPictureTaken;

protected : 
	UPROPERTY(EditAnywhere)
	class USceneCaptureComponent2D* SceneCaptureComponent;

	UPROPERTY(EditAnywhere)
	class UTextureRenderTarget2D* RenderTarget;

	UPROPERTY(EditAnywhere)
	float SpawnHeight = 3150;

	UPROPERTY(EditAnywhere)
	float CaptureWidth = 6000;

	UPROPERTY(EditAnywhere)
	float RenderTargetHeight = 512;

	UPROPERTY(EditAnywhere)
	float RenderTargetWidth = 512;

};
