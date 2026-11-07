// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EyeTrackDebugActor.generated.h"


UCLASS()
class VRFOVEATION_API AEyeTrackDebugActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEyeTrackDebugActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
    /** max raycast range */
    UPROPERTY(EditAnywhere, Category = "Gaze Settings")
    float MaxTraceDistance = 5000.0f;

    /** smooth speed to filter sensor jitter (higher = snappier, lower = smoother) */
    UPROPERTY(EditAnywhere, Category = "Gaze Settings")
    float SmoothingInterpSpeed = 25.0f;


    FVector SmoothedHitLocation;
    bool HasInitialHit = false;
};
