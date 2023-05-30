// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/EclipseSubsystem.h"

UEclipseSubsystem::UEclipseSubsystem()
{
	if (DayCycleTime < 1.0f)
		DayCycleTime = 30.0f;
	if (EclipseCycleTime < 1.0f)
		EclipseCycleTime = 60.0f;
	Accumulator = 0.0f;
	CurrentNatureState = ENatureState::Normal;
}

void UEclipseSubsystem::ToggleNatureState()
{
	switch (CurrentNatureState)
	{
		case ENatureState::Normal:
			SetCurrentState(ENatureState::Eclipse);
			break;
		case ENatureState::Eclipse:
			SetCurrentState(ENatureState::Normal);
			break;
	}
	OnNatureStateChanged.Broadcast(CurrentNatureState);
}

void UEclipseSubsystem::Tick(float DeltaTime)
{
	if(bRunning)
		Accumulator += DeltaTime;
		switch (CurrentNatureState)
		{
		case ENatureState::Normal:
			if (Accumulator >= DayCycleTime)
			{
				Accumulator = 0.0f;
				ToggleNatureState();
			}
			break;
		case ENatureState::Eclipse:
			if (Accumulator >= EclipseCycleTime)
			{
				Accumulator = 0.0f;
				ToggleNatureState();
			}
			break;
		default:
			break;
		}
}

void UEclipseSubsystem::StartRunning()
{
	bRunning = true;
}