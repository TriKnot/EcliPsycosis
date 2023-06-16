// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/EffectController.h"

void UEffectController::Init(FModifierSet _InSet, UObject* WorldContext)
{
	WorldContextObj = WorldContext;
	SpeedModifier = _InSet.SpeedModifier;
	LightAttackModifier = _InSet.LightAttackModifier;
	LightAttackEffect = _InSet.LightAttackEffect;
	HeavyAttackModifier = _InSet.HeavyAttackModifier;
	HeavyAttackEffect = _InSet.HeavyAttackEffect;
	WeaponAbilityModifier = _InSet.HeavyAttackModifier;
	WeaponAbilityEffect = _InSet.HeavyAttackEffect;
	DamageOverTime = _InSet.DamageOverTime;
	AttackEffectTime = _InSet.AttackEffectTime;
	RangeModifier = _InSet.RangeModifier;
	CooldownModifier = _InSet.CooldownModifier;
	EffectTime = _InSet.EffectTime;

	ItemEffectDelegate.BindUFunction(this, "EndEffect");
	GetWorld()->GetTimerManager().SetTimer(ItemEffectHandle, ItemEffectDelegate, _InSet.EffectTime, false);
}

float UEffectController::EffectTimeRatio() const
{
	if (ItemEffectHandle.IsValid())
		return GetWorld()->GetTimerManager().GetTimerRemaining(ItemEffectHandle)/EffectTime;
	else
		return 0.0f;
}

void UEffectController::EndEffect()
{
	if( ItemEffectHandle.IsValid() )
		GetWorld()->GetTimerManager().ClearTimer(ItemEffectHandle);
	OnEffectComplete.Broadcast(this);
}