// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon.h"
#include "Damage/DamageSystem.h"
#include "MeleeComponent.generated.h"

class UShapeComponent;

/**
 * Base Class for the Melee Weapon. 
 */
UCLASS(Blueprintable)
class ROGUELIKE_API UMeleeComponent : public UActorComponent, public IWeapon
{
	GENERATED_BODY()
public:
	/** Overriding the BeginPlay **/
	virtual void BeginPlay() override;
	
	/** Implementation of Light Attack **/
	virtual void LightAttack() override;

	/** Light Attack Functions**/
	void StartLightAttack();

	void EndLightAttack();

	/** IMplementation of Heavy Attack **/
	virtual void HeavyAttack() override;

	/** Heavy Attack Functions**/
	void StartHeavyAttack();

	void EndHeavyAttack();

	/** IMplementation of Weapon Ability **/
	virtual void WeaponAbility() override;

	/** Light Attack Functions**/
	void StartWeaponAbility();

	void EndWeaponAbility();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Elemental Effect")
	EEffectType CurrentEffectType;

	/** Configurator Variables **/

	/** Light Attack Damage **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configurator")
	float LightAttackDamage;

	/** Heavy Attack Damage **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configurator")
	float HeavyAttackDamage;

	/** Weapon Ability Damage **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configurator")
	float WeaponAbilityDamage;
public:
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	/*Hitboxes that this Weapon Component will Bind To */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UShapeComponent*> HitBoxes;

	/* Damageable Objects that have already been hit during an attack*/
	TArray<IDamageSystem*> DamageableObjects;
	
	/*Flag to Check if Attacking*/
	bool bIsAttacking;

	/*Current Damage value*/
	float CurrentDamageValue;

	
};
