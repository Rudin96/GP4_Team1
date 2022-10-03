// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MatineeCameraShake.h"
#include "GP4_CameraShake.generated.h"

/**
 * 
 */
UCLASS()
class GP4_TEAM1_API UGP4_CameraShake : public UMatineeCameraShake
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		float MinAmp = 3.0f;
	UPROPERTY(EditAnywhere)
		float MaxAmp = 6.0f;

	UPROPERTY(EditAnywhere)
		float MinFreq = 10.0f;
	UPROPERTY(EditAnywhere)
		float MaxFreq = 25.0f;

		UGP4_CameraShake();
};
