// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CustomStructs/EnumSet.h"
#include "CustomStructs/StructSet.h"
#include "DamageSystem.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UDamageSystem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROGUELIKE_API IDamageSystem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** Abstract Function for Damage Transfer **/
	UFUNCTION(BlueprintCallable, Category = "Damage System Interface")
	virtual void TransferDamage(float _DamageAmount, FAttackEffect _EffectType);
};
