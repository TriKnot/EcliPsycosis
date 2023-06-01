
// Fill out your copyright notice in the Description page of Project Settings.


#include "Damage/DamageEffect.h"

// Sets default values for this component's properties
UDamageEffect::UDamageEffect()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	EffectType = EEffectTypes::AE_None;
}

UDamageEffect::~UDamageEffect()
{
	if (DamageEffectHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(DamageEffectHandle);
}


// Called when the game starts
void UDamageEffect::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDamageEffect::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsTracking)
	{
		Accumulator += DeltaTime;
		if (Accumulator >= 1.0f)
		{
			OnDamagePerSecond.Broadcast(-DamageOverTime);
			Accumulator = 0.0f;
		}
	}
}

void UDamageEffect::StartEffectTimer(float Time)
{
	Accumulator = 0.0f;
	bIsTracking = true;
	DamageEffectDelegate.BindUFunction(this, "EndEffectTimer");
	GetWorld()->GetTimerManager().SetTimer(DamageEffectHandle, DamageEffectDelegate, Time, false);
}

void UDamageEffect::EndEffectTimer()
{
	ClearEffects();
}

void UDamageEffect::DamageReceived(float _DamageAmount, FAttackEffect _EffectType)
{
	UE_LOG(LogTemp, Warning, TEXT("Damage received"));
	ClearEffects();
	EffectType = _EffectType._EffectType;
	switch (EffectType)
	{
	case EEffectTypes::AE_None:
		ClearEffects();
		break;
	case EEffectTypes::AE_Stun:
		OnStunStateChanged.Broadcast(true);
		break;
	case EEffectTypes::AE_Poison:
	case EEffectTypes::AE_Burn:
		if (_EffectType.AttackEffectTime > 1.0f)
		{
			DamageOverTime = _EffectType.DamageOverTime;
			StartEffectTimer(_EffectType.AttackEffectTime);
		}
		break;
	default:
		ClearEffects();
	}
}

void UDamageEffect::ClearEffects()
{
	switch (EffectType)
	{
	case EEffectTypes::AE_Stun:
		OnStunStateChanged.Broadcast(false);
		break;
	default:
		break;
	}
	EffectType = EEffectTypes::AE_None;
	bIsTracking = false;
	DamageOverTime = 0.0f;
	if(DamageEffectHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(DamageEffectHandle);

}



