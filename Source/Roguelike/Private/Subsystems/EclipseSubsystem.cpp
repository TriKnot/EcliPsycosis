// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/EclipseSubsystem.h"

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