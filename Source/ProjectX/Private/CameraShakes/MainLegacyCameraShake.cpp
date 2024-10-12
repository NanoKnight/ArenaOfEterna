// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraShakes/MainLegacyCameraShake.h"

UMainLegacyCameraShake::UMainLegacyCameraShake()
{
	OscillationDuration = 0.5f;
	OscillationBlendInTime = 0.1f;
	OscillationBlendOutTime = 0.2f;


	RotOscillation.Pitch.Amplitude = 1.f;
	RotOscillation.Pitch.Frequency = 1.f;

	RotOscillation.Yaw.Amplitude = 1.f;
	RotOscillation.Yaw.Frequency = 1.f;








}
