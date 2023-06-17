// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CustomStructs/EnumSet.h"
#include "EclipseSubsystem.generated.h"

const float DAYTIME = 15.0f;
const float ECLIPSETIME  = 15.0f;

class UCustomGameInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNatureStateChanged, ENatureState, NewNatureState);
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ROGUELIKE_API UEclipseSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()


public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual void Tick(float DeltaTime) override;

	UCustomGameInstance* CurrentGI;

	UPROPERTY(BlueprintAssignable)
		FOnNatureStateChanged OnNatureStateChanged;
	UFUNCTION(BlueprintCallable, Category = "EclipseSubSystem")
		FORCEINLINE ENatureState GetCurrentState() const { return CurrentNatureState; }
	UFUNCTION(BlueprintCallable, Category = "EclipseSubSystem")
		FORCEINLINE void SetCurrentState(ENatureState _NewState) { CurrentNatureState = _NewState; }
	UFUNCTION(BlueprintCallable, Category = "EclipseSubSystem")
		FORCEINLINE float GetEclipseTime() const { return EclipseCycleTime; }
	UFUNCTION(BlueprintCallable, Category = "EclipseSubSystem")
		FORCEINLINE void SetEclipseTime(float _NewCycleTime) { EclipseCycleTime = _NewCycleTime; }
	UFUNCTION(BlueprintCallable, Category = "EclipseSubSystem")
		FORCEINLINE float GetDayTime() const { return DayCycleTime; }
	UFUNCTION(BlueprintCallable, Category = "EclipseSubSystem")
		FORCEINLINE void SetDayTime(float _NewCycleTime) { DayCycleTime = _NewCycleTime; }
	UFUNCTION()
		void ToggleNatureState();
	UFUNCTION(BlueprintCallable)
		void StartRunning();
	virtual TStatId GetStatId() const override {return TStatId();};

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	ENatureState CurrentNatureState;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	float EclipseCycleTime;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	float DayCycleTime;

	bool bRunning;
	float Accumulator;
	
};
