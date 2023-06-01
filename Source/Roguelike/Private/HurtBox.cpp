// Fill out your copyright notice in the Description page of Project Settings.


#include "HurtBox.h"

void UHurtBox::TransferDamage(float _DamageAmount, FAttackEffect _EffectType)
{
	OnReceivedDamage.Broadcast(_DamageAmount, _EffectType);
}