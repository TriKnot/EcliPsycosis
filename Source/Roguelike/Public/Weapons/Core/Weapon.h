// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Weapon.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UWeapon : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROGUELIKE_API IWeapon
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** Abstract Functions for Light Attack **/
	UFUNCTION(BlueprintCallable, Category = "Weapon Interface")
	virtual void LightAttack();

	/** Abstract Function for Heavy Attack **/
	UFUNCTION(BlueprintCallable, Category = "Weapon Interface")
	virtual void HeavyAttack();

	/** Abstract Function for Weapon Ability **/
	UFUNCTION(BlueprintCallable, Category = "Weapon Interface")
	virtual void WeaponAbility();
};
