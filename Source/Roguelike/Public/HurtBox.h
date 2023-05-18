// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Damage/DamageSystem.h"
#include "HurtBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReceivedDamage, float, DamageAmount/*, EEffectType, EffectType*/);
/**
 *
 */
UCLASS()
class ROGUELIKE_API UHurtBox : public UBoxComponent, public IDamageSystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable) //TODO: Remove Specifiers Later if not needed
		FOnReceivedDamage OnReceivedDamage;
	/** Implements TransferDamage **/
	virtual void TransferDamage(float _DamageAmount/*, EEffectType _EffectType*/) override;
};

