// Fill out your copyright notice in the Description page of Project Settings.


#include "GP4_CameraShake.h"

UGP4_CameraShake::UGP4_CameraShake()
{
	OscillationDuration = 0.25;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(MinAmp, MaxAmp);
	RotOscillation.Pitch.Frequency = FMath::RandRange(MinFreq, MaxFreq);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(MinAmp, MaxAmp);
	RotOscillation.Yaw.Frequency = FMath::RandRange(MinFreq, MaxFreq);
}

