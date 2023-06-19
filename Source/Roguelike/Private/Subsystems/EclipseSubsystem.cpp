// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/EclipseSubsystem.h"
#include "Config/CustomGameInstance.h"


void UEclipseSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	CurrentGI = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance());
	if (CurrentGI)
	{
		CurrentNatureState = CurrentGI->NatureState;
		switch (CurrentNatureState)
		{
			case ENatureState::Sun:
				DayCycleTime = CurrentGI->RemainingTime > 0.0f ? CurrentGI->RemainingTime : DAYTIME;
				break;
			case ENatureState::Eclipse:
				EclipseCycleTime = CurrentGI->RemainingTime > 0.0f ? CurrentGI->RemainingTime : ECLIPSETIME;
				break;
		}
	}
	Accumulator = 0.0f;
}

void UEclipseSubsystem::Deinitialize()
{
	bRunning = false;
	if (CurrentGI)
	{
		CurrentGI->NatureState = CurrentNatureState;
		if (CurrentNatureState == ENatureState::Sun)
		{
			CurrentGI->RemainingTime = DAYTIME - Accumulator;
		}
		else
		{
			CurrentGI->RemainingTime = ECLIPSETIME - Accumulator;
		}
	}
}


void UEclipseSubsystem::ToggleNatureState()
{
	switch (CurrentNatureState)
	{
		case ENatureState::Sun:
			EclipseCycleTime = ECLIPSETIME;
			SetCurrentState(ENatureState::Eclipse);
			break;
		case ENatureState::Eclipse:
			DayCycleTime = DAYTIME;
			SetCurrentState(ENatureState::Sun);
			break;
		default:
			SetCurrentState(ENatureState::Sun);
	}
	OnNatureStateChanged.Broadcast(CurrentNatureState);
}

void UEclipseSubsystem::Tick(float DeltaTime)
{
	if (bRunning)
	{
		Accumulator += DeltaTime;
		switch (CurrentNatureState)
		{
		case ENatureState::Sun:
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
		
}

void UEclipseSubsystem::StartRunning()
{
	bRunning = true;
}