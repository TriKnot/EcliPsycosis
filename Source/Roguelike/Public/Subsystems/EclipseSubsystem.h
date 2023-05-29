// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CustomStructs/EnumSet.h"
#include "EclipseSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNatureStateChanged, ENatureState, NewNatureState);
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ROGUELIKE_API UEclipseSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintAssignable)
		FOnNatureStateChanged OnNatureStateChanged;
	UFUNCTION(BlueprintCallable, Category = "EclipseSubSystem")
		FORCEINLINE ENatureState GetCurrentState() const { return CurrentNatureState; }
	UFUNCTION(BlueprintCallable, Category = "EclipseSubSystem")
		FORCEINLINE void SetCurrentState(ENatureState _NewState) { CurrentNatureState = _NewState; }
	UFUNCTION(BlueprintCallable, Category = "EclipseSubSystem")
		FORCEINLINE float GetCycleTime() const { return EclipseCycleTime; }
	UFUNCTION(BlueprintCallable, Category = "EclipseSubSystem")
		FORCEINLINE void SetCycleTime(float _NewCycleTime) { EclipseCycleTime = _NewCycleTime; }
	UFUNCTION()
		void ToggleNatureState();
	virtual TStatId GetStatId() const override {return TStatId();};

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	ENatureState CurrentNatureState;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	float EclipseCycleTime;
	
};
