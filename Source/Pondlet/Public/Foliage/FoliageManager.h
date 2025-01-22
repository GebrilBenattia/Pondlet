// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeatherCycleSettings.h"
#include "FoliageManager.generated.h"

UCLASS()
class PONDLET_API AFoliageManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFoliageManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Foliage")
	UStaticMeshComponent * SpawnFoliage(FTransform Transform, class UStaticMesh* FoliageMesh);

	UStaticMesh* LoadMeshFromBuffer(FSoftObjectPath AssetPath);

	UFUNCTION(BlueprintCallable)
	void ClearFoliage();

	UFUNCTION(BlueprintCallable)
	void ChangeWeather(FName NewWeather);

private : 
	void AgeFoliage(float DeltaTime);


public :  
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName CurrentWeather;

	

protected : 

	UPROPERTY(EditAnywhere)
	TArray<UStaticMesh*> ShadowlessMeshes;

	UPROPERTY(EditAnywhere)
	TArray<UStaticMesh*> GrassAndFlowers;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))

	class UWeatherCycleSettings* WeatherCycleSettings;

	FWeatherStateSettings WeatherStateSettings;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Time", meta = (AllowPrivateAccess = "true"))
	class UTimeManagerSettingsDataAsset* TimeManagerSettings;

private : 
	UPROPERTY()
	TMap<FString, UStaticMesh*> MeshBuffer;


};
