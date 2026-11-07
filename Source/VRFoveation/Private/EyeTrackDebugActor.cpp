// Fill out your copyright notice in the Description page of Project Settings.


#include "EyeTrackDebugActor.h"
#include "EyeTrackerFunctionLibrary.h"
#include "DrawDebugHelpers.h"


AEyeTrackDebugActor::AEyeTrackDebugActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEyeTrackDebugActor::BeginPlay()
{
	Super::BeginPlay();

	const bool IsConnected = UEyeTrackerFunctionLibrary::IsEyeTrackerConnected();
	UE_LOG(LogTemp, Warning, TEXT("Eye Tracker connected: %s"), IsConnected ? TEXT("TRUE") : TEXT("FALSE"));
}

void AEyeTrackDebugActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FEyeTrackerGazeData GazeData;
	const bool HasGaze = UEyeTrackerFunctionLibrary::GetGazeData(GazeData);

	if (HasGaze == false)
	{
		GEngine->AddOnScreenDebugMessage(1, 0.05f, FColor::Yellow, TEXT("Waiting for Gaze Data"));
		return;
	}

	if (GazeData.bIsLeftEyeBlink && GazeData.bIsRightEyeBlink)
	{
		return;
	}

	if (GazeData.ConfidenceValue < 0.20f)
	{
		return;
	}

    // Raycast to find the surface the user is looking at
    const FVector TraceStart = GazeData.GazeOrigin;
    const FVector TraceEnd = TraceStart + (GazeData.GazeDirection * MaxTraceDistance);

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;

    FVector TargetLocation = TraceEnd;

    UWorld* World = GetWorld();
    if (IsValid(World) == false) { return; }

    if (World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
    {
        TargetLocation = HitResult.ImpactPoint;
    }
    else if (GazeData.FixationPoint != FVector::ZeroVector)
    {
        TargetLocation = GazeData.FixationPoint;
    }

    // Smooth position to avoid micro-jitter
    if (HasInitialHit == false)
    {
        SmoothedHitLocation = TargetLocation;
        HasInitialHit = true;
    }
    else
    {
        SmoothedHitLocation = FMath::VInterpTo(SmoothedHitLocation, TargetLocation, DeltaTime, SmoothingInterpSpeed);
    }

    DrawDebugSphere(World,  SmoothedHitLocation, 4.0f, 16, FColor::Cyan, false, -1.0f, 0, 1.0f);
}

