// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CustomStructs/StructSet.h"
#include "CustomStructs/EnumSet.h"
#include "EffectController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectComplete, UEffectController*, ElapsedController);
/**
 * 
 */
UCLASS(Blueprintable)
class ROGUELIKE_API UEffectController : public UObject
{
	GENERATED_BODY()
public:
	//Init Function
	void Init(FModifierSet _InSet, UObject* WorldContext);

	UPROPERTY(BlueprintAssignable)
	FOnEffectComplete OnEffectComplete;

	UFUNCTION(BlueprintCallable)
	float EffectTimeRatio() const;

	UFUNCTION()
	void EndEffect();

	UObject* WorldContextObj;

	FTimerHandle ItemEffectHandle;
	FTimerDelegate ItemEffectDelegate;
	//Variables
	float SpeedModifier;
	float LightAttackModifier;
	EEffectTypes LightAttackEffect;
	float HeavyAttackModifier;
	EEffectTypes HeavyAttackEffect;
	float WeaponAbilityModifier;
	EEffectTypes WeaponAbilityEffect;
	float DamageOverTime;
	float AttackEffectTime;
	float RangeModifier;
	float CooldownModifier;
	float EffectTime;
	
};
