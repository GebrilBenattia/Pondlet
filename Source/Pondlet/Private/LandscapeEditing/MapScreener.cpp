// Fill out your copyright notice in the Description page of Project Settings.


#include "LandscapeEditing/MapScreener.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/Pondlet_GameState.h"


// Sets default values
AMapScreener::AMapScreener()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	SceneCaptureComponent->SetupAttachment(RootComponent);
	SceneCaptureComponent->bCaptureEveryFrame = false;
	SceneCaptureComponent->bCaptureOnMovement = false;

	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> RenderTargetFinder(TEXT("/Game/Assets/Texture/RenderTarget/RT_MapBirdView.RT_MapBirdView"));
	if (RenderTargetFinder.Succeeded()) {
		RenderTarget = RenderTargetFinder.Object;
	}
}

void AMapScreener::TakeScreenShot()
{
	if (RenderTarget != nullptr)
	{
		RenderTarget->ResizeTarget(RenderTargetWidth, RenderTargetHeight);
		SceneCaptureComponent->TextureTarget = RenderTarget;
		SceneCaptureComponent->CaptureScene();
		/*RenderTarget -> */
		OnPictureTaken.ExecuteIfBound();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RenderTarget is null"));
	}
}

// Called when the game starts or when spawned
void AMapScreener::BeginPlay()
{
	Super::BeginPlay();
	FVector Location = GetActorLocation();
	Location.Z = SpawnHeight;
	SetActorLocation(Location);
	if (SceneCaptureComponent) {
		SceneCaptureComponent->OrthoWidth = CaptureWidth;
	}
}

