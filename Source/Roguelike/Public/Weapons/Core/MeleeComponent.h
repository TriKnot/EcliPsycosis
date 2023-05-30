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
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnAttack OnLightAttack;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnAttack OnHeavyAttack;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnAttack OnWeaponAbility;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FAttackStateChanged OnAttackStateChanged;
	/** Overriding the BeginPlay **/
	virtual void BeginPlay() override;

	/** Functions to Start and Stop Attack State **/

	UFUNCTION()
	virtual void StartAttack() override;

	UFUNCTION()
	virtual void StopAttack() override;

	UFUNCTION()
	virtual void ClearAttack() override;

	/** Implementation of Light Attack **/
	virtual void LightAttack() override;
	
	/** IMplementation of Heavy Attack **/
	virtual void HeavyAttack() override;

	/** IMplementation of Weapon Ability **/
	virtual void WeaponAbility() override;

	/** Toggle Hitbox on and off */
	UFUNCTION(BlueprintCallable)
	void ToggleHitBox(bool bIsEnabled);
	
	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Elemental Effect")
	EEffectType CurrentEffectType;*/

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

	/** Attack Modifier**/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Configurator")
	float AttackModifier;

	/** Types that this component can attack and damage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configurator")
	ECanDamageTypes CanDamageTypes;
	
public:
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void DamageInRangeActors();
	
private:
	/*Hit Boxes that this Weapon Component will Bind To */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UShapeComponent*> HitBoxes;

	/* Damageable Objects that have already been hit during an attack*/
	UPROPERTY()
	TArray<AActor*> DamagedActors;

	TArray<IDamageSystem*> InRangeActors;

	/*Current Damage value*/
	float CurrentDamageValue;

private:
	// Returns true if the actor is a valid target for this component
	// If true, OutDamageSystem will be set to the damage system of the actor
	bool TryGetDamageSystem(UPrimitiveComponent* OtherComponent, IDamageSystem*& OutDamageSystem) const;
	
};

///** Light Attack Functions**/
	//UFUNCTION(BlueprintCallable)
	//void StartLightAttack();

	//UFUNCTION(BlueprintCallable)
	//void EndLightAttack();

///** Heavy Attack Functions**/
	//UFUNCTION(BlueprintCallable)
	//void StartHeavyAttack();

	//UFUNCTION(BlueprintCallable)
	//void EndHeavyAttack();

	///** Light Attack Functions**/
	//UFUNCTION(BlueprintCallable)
	//void StartWeaponAbility();

	//UFUNCTION(BlueprintCallable)
	//void EndWeaponAbility();
