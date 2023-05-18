// Fill out your copyright notice in the Description page of Project Settings.


#include "HurtBox.h"

void UHurtBox::TransferDamage(float _DamageAmount/*, EEffectType _EffectType*/)
{
	OnReceivedDamage.Broadcast(_DamageAmount/*, _EffectType*/);
}