// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CustomStructs/EnumSet.h"
#include "CustomStructs/StructSet.h"
#include "DamageEffect.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPeriodicDamage, float, DamagePercentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectState, bool, NewState);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKE_API UDamageEffect : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamageEffect();

protected:
	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Effect Timers
	FTimerHandle DamageEffectHandle;
	FTimerDelegate DamageEffectDelegate;


	//Start the Effect Timer
	UFUNCTION(BlueprintCallable)
	void StartEffectTimer(float Time);

	//Stop the Effect Timer
	UFUNCTION()
	void EndEffectTimer();

	//Receive Damage Effect
	UFUNCTION()
	void DamageReceived(float _DamageAmount, FAttackEffect _EffectType);

	//Clear Effects
	UFUNCTION()
	void ClearEffects();

	UPROPERTY(BlueprintAssignable)
	FOnPeriodicDamage OnDamagePerSecond;

	UPROPERTY(BlueprintAssignable)
	FOnEffectState OnStunStateChanged;

private:
	//Tracking timer for Damage over Time
	bool bIsTracking;

	float Accumulator;

	float DamageOverTime;

	EEffectTypes EffectType;

};
